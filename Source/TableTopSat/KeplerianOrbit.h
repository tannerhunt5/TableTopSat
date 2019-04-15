// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <math.h>
#include <array>
#include <tuple>
#include "DrawDebugHelpers.h"
#include "GameFramework/Actor.h"
#include "KeplerianOrbit.generated.h"

UCLASS()
class TABLETOPSAT_API AKeplerianOrbit : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeplerianOrbit();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Constants
	float mu = 398600.4418;
	float small = .000001;
	float pi = 3.14159265358979; // pi to 15 digits


	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "0", ClampMax = "1.0"))
	float Eccentricity = 0;
	
	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "0"))
	float SemiMajorAxis = 0;

	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "0", ClampMax = "180"))
	float Inclination = 0;

	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "0", ClampMax = "360"))
	float RAAN = 0;

	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "0", ClampMax = "360"))
	float ArgOfPeriapsis = 0;

	UPROPERTY(EditAnywhere, Category = "Orbit Parameters", meta = (ClampMin = "10", ClampMax = "1000"))
	int NumberOfPoints = 10;

	// Semi-latus Rectum
	float p = 0;

	// True Anomaly - will loop from 0-2pi to find discrete points along orbit
	TArray<float> nu;


	float FindSemiLatusRectum(float a, float e);

	TArray<FVector> COE2RV(float p, float ecc, float incl, float omega, float argp, float nu);

	TArray<float> CreateNuArray();

	FVector rot1(FVector vec, float xval);
	FVector outvec1;

	FVector rot3(FVector vec, float xval);
	FVector outvec3;

	// Final State Vectors in IJK frame
	TArray<FVector> R_ijk;
	TArray<FVector> V_ijk;

	TArray<FVector> TempState;

	TArray<float> temp;

	float EccentricityTemp;
	float SemiMajorAxisTemp;
	float InclinationTemp;
	float RAANTemp;
	float ArgOfPeriapsisTemp;
	float NumberOfPointsTemp;

	// Testing pointer stuff
	float TwoPlusTwo();
	
	float Period;
};
