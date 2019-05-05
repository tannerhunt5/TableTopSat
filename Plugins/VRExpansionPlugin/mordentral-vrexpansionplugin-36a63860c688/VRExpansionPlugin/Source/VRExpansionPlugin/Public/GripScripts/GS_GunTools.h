// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRGripScriptBase.h"
#include "GripScripts/GS_Default.h"
#include "GS_GunTools.generated.h"

class UGripMotionControllerComponent;

// Event thrown when we enter into virtual stock mode
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVRVirtualStockModeChangedSignature, bool, IsVirtualStockEngaged);

// Global settings for this player
USTRUCT(BlueprintType, Category = "GunSettings")
struct VREXPANSIONPLUGIN_API FBPVirtualStockSettings
{
	GENERATED_BODY()
public:

	// *Global Value* Should we auto snap to the virtual stock by a set distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock")
		bool bUseDistanceBasedStockSnapping;

	// *Global Value* The distance before snapping to the stock / unsnapping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock")
		float StockSnapDistance;

	// *Global Value* The distance from the edge of the stock snap distance where it will be at 100% influence
	// Prior to this threshold being hit it will lerp from standard hold to the virtual stock version.
	// A value of 0.0f will leave it always off
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock", meta = (ClampMin = "0.00", UIMin = "0.00"))
		float StockSnapLerpThreshold;

	// Current lerp value of the stock from zero influence to full influence
	UPROPERTY(BlueprintReadOnly, Category = "VirtualStock")
		float StockLerpValue;

	// *Global Value* An offset to apply to the HMD location to be considered the neck / mount pivot 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock")
		FVector_NetQuantize100 StockSnapOffset;

	// *Global Value* Whether we should lerp the location of the rearmost (stock side) hand, mostly used for snipers.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock|Smoothing")
		bool bSmoothStockHand;

	// *Global Value* How much influence the virtual stock smoothing should have, 0.0f is zero smoothing, 1.0f is full smoothing, you should test with full smoothing to get the amount you
	// want and then set the smoothing value up until it feels right between the fully smoothed and unsmoothed values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VirtualStock|Smoothing", meta = (editcondition = "bSmoothStockHand", ClampMin = "0.00", UIMin = "0.00", ClampMax = "1.00", UIMax = "1.00"))
		float SmoothingValueForStock;

	// Used to smooth filter the virtual stocks primary hand location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Smoothing")
		FBPEuroLowPassFilter StockHandSmoothing;

	// Draw debug elements showing the virtual stock location and angles to interacting components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Debug")
	bool bDebugDrawVirtualStock;

	void CopyFrom(FBPVirtualStockSettings & B)
	{
		bUseDistanceBasedStockSnapping = B.bUseDistanceBasedStockSnapping;
		StockSnapDistance = B.StockSnapDistance;
		StockSnapLerpThreshold = B.StockSnapLerpThreshold;
		StockSnapOffset = B.StockSnapOffset;
		bSmoothStockHand = B.bSmoothStockHand;
		SmoothingValueForStock = B.SmoothingValueForStock;
		StockHandSmoothing = B.StockHandSmoothing;
	}

	FBPVirtualStockSettings()
	{
		StockSnapOffset = FVector(0.f, 0.f, 0.f);
		StockSnapDistance = 35.f;
		StockSnapLerpThreshold = 20.0f;
		StockLerpValue = 0.0f;
		bUseDistanceBasedStockSnapping = true;
		SmoothingValueForStock = 0.0f;
		bSmoothStockHand = false;

		// Speed up the lerp on fast movements for this
		StockHandSmoothing.DeltaCutoff = 20.0f;
		StockHandSmoothing.MinCutoff = 5.0f;

		bDebugDrawVirtualStock = false;
	}
};

USTRUCT(BlueprintType, Category = "VRExpansionLibrary")
struct VREXPANSIONPLUGIN_API FGunTools_AdvSecondarySettings
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings")
		bool bUseAdvancedSecondarySettings;

	// Scaler used for handling the smoothing amount, 0.0f is zero smoothing, 1.0f is full smoothing, you should test with full smoothing to get the amount you
	// want and then set the smoothing value up until it feels right between the fully smoothed and unsmoothed values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|Smoothing", meta = (editcondition = "bUseAdvancedSecondarySettings", ClampMin = "0.00", UIMin = "0.00", ClampMax = "1.00", UIMax = "1.00"))
		float SecondaryGripScaler;

	// If true we will constantly be lerping with the grip scaler instead of only sometimes.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|Smoothing", meta = (editcondition = "bUseAdvancedSecondarySettings"))
		bool bUseConstantGripScaler;

	// If true will override custom settings for the smoothing values with the global settings in VRSettings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|Smoothing", meta = (editcondition = "bUseAdvancedSecondarySettings"))
		bool bUseGlobalSmoothingSettings;

	// Used to smooth filter the secondary influence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|Smoothing")
		FBPEuroLowPassFilter SecondarySmoothing;

	// Whether to scale the secondary hand influence off of distance from grip point
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|DistanceInfluence", meta = (editcondition = "bUseAdvancedSecondarySettings"))
		bool bUseSecondaryGripDistanceInfluence;

	// If true, will use the GripInfluenceDeadZone as a constant value instead of calculating the distance and lerping, lets you define a static influence amount.
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SecondaryGripSettings", meta = (editcondition = "bUseSecondaryGripDistanceInfluence"))
	//	bool bUseGripInfluenceDeadZoneAsConstant;

	// Distance from grip point in local space where there is 100% influence
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|DistanceInfluence", meta = (editcondition = "bUseSecondaryGripDistanceInfluence", ClampMin = "0.00", UIMin = "0.00", ClampMax = "256.00", UIMax = "256.00"))
		float GripInfluenceDeadZone;

	// Distance from grip point in local space before all influence is lost on the secondary grip (1.0f - 0.0f influence over this range)
	// this comes into effect outside of the deadzone
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AdvSecondarySettings|DistanceInfluence", meta = (editcondition = "bUseSecondaryGripDistanceInfluence", ClampMin = "1.00", UIMin = "1.00", ClampMax = "256.00", UIMax = "256.00"))
		float GripInfluenceDistanceToZero;

	FGunTools_AdvSecondarySettings()
	{
		bUseAdvancedSecondarySettings = false;
		SecondaryGripScaler = 0.0f;
		bUseGlobalSmoothingSettings = true;
		bUseSecondaryGripDistanceInfluence = false;
		//bUseGripInfluenceDeadZoneAsConstant(false),
		GripInfluenceDeadZone = 50.0f;
		GripInfluenceDistanceToZero = 100.0f;
		bUseConstantGripScaler = false;
	}
};


// A grip script that adds useful fire-arm related features to grips
// Just adding it to the grippable object provides the features without removing standard
// Grip features.
UCLASS(NotBlueprintable, ClassGroup = (VRExpansionPlugin))
class VREXPANSIONPLUGIN_API UGS_GunTools : public UGS_Default
{
	GENERATED_BODY()
public:

	UGS_GunTools(const FObjectInitializer& ObjectInitializer);

	virtual void OnGrip_Implementation(UGripMotionControllerComponent * GrippingController, const FBPActorGripInformation & GripInformation) override;
	virtual void OnSecondaryGrip_Implementation(UGripMotionControllerComponent * Controller, USceneComponent * SecondaryGripComponent, const FBPActorGripInformation & GripInformation) override;

	// (default false) If true will run through the entire simulation that the owning client uses for the gun. If false, does a lighter and more performant approximation.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunSettings")
		bool bUseHighQualityRemoteSimulation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings")
	FGunTools_AdvSecondarySettings AdvSecondarySettings;

	// Offset to apply to the pivot (good for centering pivot into the palm ect).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Pivot")
		FVector_NetQuantize100 PivotOffset;

	UFUNCTION(BlueprintCallable, Category = "GunTools|VirtualStock")
		void SetVirtualStockComponent(USceneComponent * NewStockComponent)
	{
		VirtualStockComponent = NewStockComponent;
	}

	UFUNCTION(BlueprintCallable, Category = "GunTools|VirtualStock")
		void SetVirtualStockEnabled(bool bAllowVirtualStock)
	{
		if (!bUseVirtualStock && bAllowVirtualStock)
			ResetStockVariables();

		bUseVirtualStock = bAllowVirtualStock;
	}

	void ResetStockVariables()
	{
		VirtualStockSettings.StockHandSmoothing.ResetSmoothingFilter();
	}

	void GetVirtualStockTarget(UGripMotionControllerComponent * GrippingController);

	// Call to use an object
	UPROPERTY(BlueprintAssignable, Category = "GunSettings|VirtualStock")
		FVRVirtualStockModeChangedSignature OnVirtualStockModeChanged;

	// Overrides the pivot location to be at this component instead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
		bool bUseVirtualStock;

	// Draw debug elements showing the virtual stock location and angles to interacting components
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Debug")
	//	bool bDebugDrawVirtualStock;

	FTransform MountWorldTransform;
	bool bIsMounted;
	TWeakObjectPtr<USceneComponent> CameraComponent;

	// Overrides the default behavior of using the HMD location for the stock and uses this component instead
	UPROPERTY(BlueprintReadWrite)
		TWeakObjectPtr<USceneComponent> VirtualStockComponent;

	// Loads the global virtual stock settings on grip (only if locally controlled, you need to manually replicate and store the global settings
	// In the character if networked).
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
		bool bUseGlobalVirtualStockSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock", meta = (editcondition = "!bUseGlobalVirtualStockSettings"))
		FBPVirtualStockSettings VirtualStockSettings;

/*
	// Should we auto snap to the virtual stock by a set distance
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
		bool bUseDistanceBasedStockSnapping;

	// The distance before snapping to the stock / unsnapping
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
	 float StockSnapDistance;

	// An offset to apply to the HMD location to be considered the neck / mount pivot 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
		FVector_NetQuantize100 StockSnapOffset;

	// Attaches the grip location to the virtual stock location (locationally).
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock")
	//	bool bAttachGripToStockLocation;

	// Overrides the pivot location to be at this component instead
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Smoothing")
		bool bSmoothStockHand;

	// How much influence the virtual stock smoothing should have, 0.0f is zero smoothing, 1.0f is full smoothing, you should test with full smoothing to get the amount you
	// want and then set the smoothing value up until it feels right between the fully smoothed and unsmoothed values.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Smoothing", meta = (editcondition = "bSmoothStockHand", ClampMin = "0.00", UIMin = "0.00", ClampMax = "1.00", UIMax = "1.00"))
		float SmoothingValueForStock;
	
	// Used to smooth filter the virtual stocks primary hand location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|VirtualStock|Smoothing")
	FBPEuroLowPassFilter StockHandSmoothing;
	*/

	// If this gun has recoil
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil")
		bool bHasRecoil;

	// Maximum recoil addition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil", meta = (editcondition = "bHasRecoil"))
		FVector_NetQuantize100 MaxRecoilTranslation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil", meta = (editcondition = "bHasRecoil"))
		FVector_NetQuantize100 MaxRecoilRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil", meta = (editcondition = "bHasRecoil"))
		FVector_NetQuantize100 MaxRecoilScale;

	// Recoil decay rate, how fast it decays back to baseline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil", meta = (editcondition = "bHasRecoil"))
		float DecayRate;

	// Recoil lerp rate, how long it takes to lerp to the target recoil amount (0.0f would be instant)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunSettings|Recoil", meta = (editcondition = "bHasRecoil"))
		float LerpRate;

	// Stores the current amount of recoil
	FTransform BackEndRecoilStorage;

	// Stores the target amount of recoil
	FTransform BackEndRecoilTarget;

	bool bHasActiveRecoil;
	
	UFUNCTION(BlueprintCallable, Category = "GunTools|Recoil")
		void AddRecoilInstance(const FTransform & RecoilAddition);

	UFUNCTION(BlueprintCallable, Category = "GunTools|Recoil")
		void ResetRecoil();

	virtual bool GetWorldTransform_Implementation(UGripMotionControllerComponent * GrippingController, float DeltaTime, FTransform & WorldTransform, const FTransform &ParentTransform, FBPActorGripInformation &Grip, AActor * actor, UPrimitiveComponent * root, bool bRootHasInterface, bool bActorHasInterface, bool bIsForTeleport) override;
	
	inline void GunTools_ApplySmoothingAndLerp(FBPActorGripInformation & Grip, FVector &frontLoc, FVector & frontLocOrig, float DeltaTime, bool bSkipHighQualitySimulations)
	{
		if (Grip.SecondaryGripInfo.GripLerpState == EGripLerpState::StartLerp) // Lerp into the new grip to smooth the transition
		{
			if (!bSkipHighQualitySimulations && AdvSecondarySettings.SecondaryGripScaler < 1.0f)
			{
				FVector SmoothedValue = AdvSecondarySettings.SecondarySmoothing.RunFilterSmoothing(frontLoc, DeltaTime);

				frontLoc = FMath::Lerp(frontLoc, SmoothedValue, AdvSecondarySettings.SecondaryGripScaler);
			}

			Default_ApplySmoothingAndLerp(Grip, frontLoc, frontLocOrig, DeltaTime);
		}
		else if (!bSkipHighQualitySimulations && AdvSecondarySettings.bUseAdvancedSecondarySettings && AdvSecondarySettings.bUseConstantGripScaler) // If there is a frame by frame lerp
		{
			FVector SmoothedValue = AdvSecondarySettings.SecondarySmoothing.RunFilterSmoothing(frontLoc, DeltaTime);

			frontLoc = FMath::Lerp(frontLoc, SmoothedValue, AdvSecondarySettings.SecondaryGripScaler);
		}
	}
};

