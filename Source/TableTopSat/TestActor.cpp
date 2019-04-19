// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"
#include "EngineUtils.h"
#include "Engine.h"
#include "KeplerianOrbit.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	/*
	AKeplerianOrbit* Orbit = Cast<AKeplerianOrbit>(OrbitPtr);*/
	

	if (OrbitPtr)
	{
		UE_LOG(LogTemp, Warning, TEXT("KeplerianOrbit Actor accessed"));
		accessed = true;

	}
	else
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Getting KeplerianOrbitActor did not work"));
	}

	//UE_LOG(LogTemp, Warning, TEXT("Array of True Anomaly's: %f"), MyTestResult);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (accessed)
	{
		rtemp = OrbitPtr->R_ijk;
		vtemp = OrbitPtr->V_ijk;

		if (rtemp.Num() > 0)
		{
			//UE_LOG(LogTemp, Warning, TEXT("R_ijk in KeplerianOrbit is %s"), *rtemp[0].ToString());
			//InitialLocation = rtemp[0];

			OrbitLocation = GetActorLocation();

			////FVector Interped = FMath::VInterpConstantTo(rtemp[0], rtemp[1], DeltaTime, vtemp[0].Size()*.1);
			//NewOrbitLocation = FMath::VInterpConstantTo(OrbitLocation, rtemp[1], DeltaTime,vtemp[0].Size()*.1);
			//SetActorLocation(NewOrbitLocation);


			for(int i = 0; i < rtemp.Num(); i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("i is %f"), i);
				if (i != rtemp.Num() - 1)
				{
					if (NewOrbitLocation != rtemp[i+1])
					{
						NewOrbitLocation = FMath::VInterpConstantTo(OrbitLocation, rtemp[i+1], DeltaTime, vtemp[i].Size()*.1);
						SetActorLocation(NewOrbitLocation);
						//UE_LOG(LogTemp, Warning, TEXT("Current Location is %s"), *NewOrbitLocation.ToString());
						UE_LOG(LogTemp, Warning, TEXT("rtemp[i+1] is %s"), *rtemp[i+1].ToString());
					}
					else
					{
						i++;
						UE_LOG(LogTemp, Warning, TEXT("i++ ran is %f"), i);

					}
				}
			}
		}
	}
}

