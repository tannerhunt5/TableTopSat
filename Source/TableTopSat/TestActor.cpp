// Fill out your copyright notice in the Description page of Project Settings.

#include "TestActor.h"
#include "EngineUtils.h"
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
		if (rtemp.Num() > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("R_ijk in KeplerianOrbit is %s"), *rtemp[0].ToString());
		}
		
	}
}

