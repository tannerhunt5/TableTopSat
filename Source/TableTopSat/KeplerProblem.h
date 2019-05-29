// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DrawDebugHelpers.h"
#include "KeplerianOrbit.h"
#include "KeplerProblem.generated.h"

UCLASS()
class TABLETOPSAT_API AKeplerProblem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKeplerProblem();

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

	int NumFrame = 0;

	// Initial r0 and v0 for ISS
	FVector r_init = { 61.783, -5.188, 6.0 };
	FVector v_init = { -6.709, -79.9, -2.0 };

	FVector r_ijk0;
	FVector v_ijk0;

	FVector r_ijk;
	FVector v_ijk;

	FVector r_current;
	FVector v_current;



	void Kepler(float dt0, FVector r0, FVector v0);

	void FindC2C3(float znew, float& c2new, float& c3new);

};
