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

			OrbitLocation = FVector(87.750000, 74.712997, 48.677002);//GetActorLocation();
	/*		X = 73.037 Y = -38.659 Z = -49.075
				(X = 117.750000, Y = 14.713000, Z = 18.677000)*/

			
			////FVector Interped = FMath::VInterpConstantTo(rtemp[0], rtemp[1], DeltaTime, vtemp[0].Size()*.1);
			//NewOrbitLocation = FMath::VInterpConstantTo(OrbitLocation, rtemp[1], DeltaTime,vtemp[0].Size()*.1);
			//SetActorLocation(NewOrbitLocation);

			UE_LOG(LogTemp, Warning, TEXT("Index of closest point is %d"), GetClosestPoint());
			for(int i = 0; i < rtemp.Num(); i++)
			{
			
				//UE_LOG(LogTemp, Warning, TEXT("i is %f"), i);
				if (i != rtemp.Num() - 1)
				{
					
					if (OrbitLocation.X - rtemp[i+1].X > .001
						&& OrbitLocation.Y - rtemp[i+1].Y > .001 
						&& OrbitLocation.Z - rtemp[i+1].Z > .001)
					{
						OrbitLocation = FMath::VInterpConstantTo(OrbitLocation, rtemp[i+1], DeltaTime,vtemp[0].Size()*.1);
						SetActorLocation(OrbitLocation);
						//UE_LOG(LogTemp, Warning, TEXT("check %s, %d"), *rtemp[i+1].ToString(), i);
					}
					else { i++; }
				}
			}
		}
	}
}

int ATestActor::GetClosestPoint()
{
	FVector TestLocation = GetActorLocation();

	for (int i = 0; i < rtemp.Num(); i++)
	{
		FVector VectDiff = TestLocation - rtemp[i];
		float tempval = VectDiff.Size();
		
		Distances.Add(tempval);

		//minimum = Distances[i];
		
		if (Distances[i] - minimum <= .001)
		{
			minimum = Distances[i];
			MinIndex = i;
			UE_LOG(LogTemp, Error, TEXT("min index = %d"), MinIndex);
		}
	}

	return MinIndex;
}

