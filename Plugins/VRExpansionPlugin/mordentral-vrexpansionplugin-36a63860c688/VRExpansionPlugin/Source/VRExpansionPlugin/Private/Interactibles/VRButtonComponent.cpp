// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "Interactibles/VRButtonComponent.h"
#include "GameFramework/Character.h"

  //=============================================================================
UVRButtonComponent::UVRButtonComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	this->SetGenerateOverlapEvents(true);
	this->PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = true;

	LastToggleTime = 0.0f;
	DepressDistance = 8.0f;
	ButtonEngageDepth = 8.0f;
	DepressSpeed = 50.0f;

	ButtonAxis = EVRInteractibleAxis::Axis_Z;
	ButtonType = EVRButtonType::Btn_Toggle_Return;

	MinTimeBetweenEngaging = 0.1f;

	bIsEnabled = true;

	this->SetCollisionResponseToAllChannels(ECR_Overlap);

	bSkipOverlapFiltering = false;
}

//=============================================================================
UVRButtonComponent::~UVRButtonComponent()
{
}

void UVRButtonComponent::BeginPlay()
{
	// Call the base class 
	Super::BeginPlay();

	ResetInitialButtonLocation();
	SetButtonToRestingPosition();

	OnComponentBeginOverlap.AddUniqueDynamic(this, &UVRButtonComponent::OnOverlapBegin);
	OnComponentEndOverlap.AddUniqueDynamic(this, &UVRButtonComponent::OnOverlapEnd);
}

void UVRButtonComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// Call supers tick (though I don't think any of the base classes to this actually implement it)
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float WorldTime = GetWorld()->GetTimeSeconds();

	if (LocalInteractingComponent.IsValid())
	{
		// If button was set to inactive during use
		if (!bIsEnabled)
		{
			// Remove interacting component and return, next tick will begin lerping back
			LocalInteractingComponent.Reset();
			return;
		}

		FTransform OriginalBaseTransform = CalcNewComponentToWorld(InitialRelativeTransform);

		float CheckDepth = FMath::Clamp(GetAxisValue(InitialLocation) - GetAxisValue(OriginalBaseTransform.InverseTransformPosition(LocalInteractingComponent->GetComponentLocation())), 0.0f, DepressDistance);

		if (CheckDepth > 0.0f)
		{

			float ClampMinDepth = 0.0f;
			
			// If active and a toggled stay, then clamp min to the toggled stay location
			if (ButtonType == EVRButtonType::Btn_Toggle_Stay && bButtonState)
				ClampMinDepth = -(ButtonEngageDepth + (1.e-2f)); // + NOT_SO_KINDA_SMALL_NUMBER

			float NewDepth = FMath::Clamp(GetAxisValue(InitialComponentLoc) + (-CheckDepth), -DepressDistance, ClampMinDepth);
			this->SetRelativeLocation(InitialRelativeTransform.TransformPosition(SetAxisValue(NewDepth)), false);

			if (ButtonType == EVRButtonType::Btn_Toggle_Return || ButtonType == EVRButtonType::Btn_Toggle_Stay)
			{
				if (!bToggledThisTouch && NewDepth <= (-ButtonEngageDepth) + KINDA_SMALL_NUMBER && (WorldTime - LastToggleTime) >= MinTimeBetweenEngaging)
				{
					LastToggleTime = WorldTime;
					bToggledThisTouch = true;
					bButtonState = !bButtonState;
					ReceiveButtonStateChanged(bButtonState, LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
					OnButtonStateChanged.Broadcast(bButtonState, LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
				}
			}
		}
	}
	else
	{
		// Std precision tolerance should be fine
		if (this->RelativeLocation.Equals(GetTargetRelativeLocation()))
		{
			this->SetComponentTickEnabled(false);

			OnButtonEndInteraction.Broadcast(LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
			ReceiveButtonEndInteraction(LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());

			LocalInteractingComponent.Reset(); // Just reset it here so it only does it once
			LocalLastInteractingComponent.Reset();
		}
		else
			this->SetRelativeLocation(FMath::VInterpConstantTo(this->RelativeLocation, GetTargetRelativeLocation(), DeltaTime, DepressSpeed), false);
	}


	// Press buttons always get checked, both during press AND during lerping for if they are active or not.
	if (ButtonType == EVRButtonType::Btn_Press)
	{
		// Check for if we should set the state of the button, done here as for the press button the lerp counts for input
		bool bCheckState = (GetAxisValue(InitialRelativeTransform.InverseTransformPosition(this->RelativeLocation)) <= (-ButtonEngageDepth) + KINDA_SMALL_NUMBER);
		if (bButtonState != bCheckState && (WorldTime - LastToggleTime) >= MinTimeBetweenEngaging)

		{
			LastToggleTime = WorldTime;
			bButtonState = bCheckState;
			ReceiveButtonStateChanged(bButtonState, LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
			OnButtonStateChanged.Broadcast(bButtonState, LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
		}
	}

}

bool UVRButtonComponent::IsValidOverlap_Implementation(UPrimitiveComponent * OverlapComponent)
{

	// Early out on the simple checks
	if (!OverlapComponent || OverlapComponent == GetAttachParent() || OverlapComponent->GetAttachParent() == GetAttachParent())
		return false;

	// Should return faster checking for owning character
	AActor * OverlapOwner = OverlapComponent->GetOwner();
	if (OverlapOwner && OverlapOwner->IsA(ACharacter::StaticClass()))
		return true;

	// Because epic motion controllers are not owned by characters have to check here too in case someone implements it like that
	// Now since our grip controllers are a subclass to the std ones we only need to check for the base one instead of both.
	USceneComponent * OurAttachParent = OverlapComponent->GetAttachParent();
	if (OurAttachParent && OurAttachParent->IsA(UMotionControllerComponent::StaticClass()))
		return true;

	// Now check for if it is a grippable object and if it is currently held
	if (OverlapComponent->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass()))
	{
		UGripMotionControllerComponent *Controller;
		bool bIsHeld;
		IVRGripInterface::Execute_IsHeld(OverlapComponent, Controller, bIsHeld);

		if (bIsHeld)
			return true;
	}
	else if(OverlapOwner && OverlapOwner->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass()))
	{
		UGripMotionControllerComponent *Controller;
		bool bIsHeld;
		IVRGripInterface::Execute_IsHeld(OverlapOwner, Controller, bIsHeld);

		if (bIsHeld)
			return true;
	}

	return false;
}

void UVRButtonComponent::SetLastInteractingActor()
{

	// Early out on the simple checks
	if (!LocalInteractingComponent.IsValid() || LocalInteractingComponent == GetAttachParent() || LocalInteractingComponent->GetAttachParent() == GetAttachParent())
	{
		LocalLastInteractingActor.Reset();
		LocalLastInteractingComponent.Reset();
		return;
	}

	LocalLastInteractingComponent = LocalInteractingComponent;

	// Should return faster checking for owning character
	AActor * OverlapOwner = LocalInteractingComponent->GetOwner();
	if (OverlapOwner && OverlapOwner->IsA(ACharacter::StaticClass()))
	{
		LocalLastInteractingActor = OverlapOwner;
		return;
	}

	// Now check for if it is a grippable object and if it is currently held
	if (LocalInteractingComponent->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass()))
	{
		UGripMotionControllerComponent *Controller;
		bool bIsHeld;
		IVRGripInterface::Execute_IsHeld(LocalLastInteractingComponent.Get(), Controller, bIsHeld);

		if (bIsHeld && Controller && Controller->GetOwner())
		{
			LocalLastInteractingActor = Controller->GetOwner();
			return;
		}
	}
	else if (OverlapOwner && OverlapOwner->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass()))
	{
		UGripMotionControllerComponent *Controller;
		bool bIsHeld;
		IVRGripInterface::Execute_IsHeld(OverlapOwner, Controller, bIsHeld);

		if (bIsHeld && Controller && Controller->GetOwner())
		{
			LocalLastInteractingActor = Controller->GetOwner();
			return;
		}
	}

	// Fall back to the owner, wasn't held and wasn't a character
	if (OverlapOwner)
	{
		LocalLastInteractingActor = OverlapOwner;
		return;
	}

	LocalLastInteractingActor.Reset();
	return;
}

void UVRButtonComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.  
	if (bIsEnabled && !LocalInteractingComponent.IsValid() && (bSkipOverlapFiltering || IsValidOverlap(OtherComp)))
	{
		LocalInteractingComponent = OtherComp;

		FTransform OriginalBaseTransform = CalcNewComponentToWorld(InitialRelativeTransform);
		FVector loc = LocalInteractingComponent->GetComponentLocation();
		InitialLocation = OriginalBaseTransform.InverseTransformPosition(LocalInteractingComponent->GetComponentLocation());
		InitialComponentLoc = OriginalBaseTransform.InverseTransformPosition(this->GetComponentLocation());
		bToggledThisTouch = false;

		this->SetComponentTickEnabled(true);

		if (LocalInteractingComponent != LocalLastInteractingComponent.Get())
		{
			SetLastInteractingActor();
			OnButtonBeginInteraction.Broadcast(LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
			ReceiveButtonBeginInteraction(LocalLastInteractingActor.Get(), LocalLastInteractingComponent.Get());
		}
	}
}

void UVRButtonComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (LocalInteractingComponent.IsValid() && OtherComp == LocalInteractingComponent)
	{
		LocalInteractingComponent.Reset();
	}
}
