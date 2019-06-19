// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <math.h>
#include "GameFramework/Actor.h"
#include "Satellite.generated.h"

UCLASS()
class TABLETOPSAT_API ASatellite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASatellite();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Constants
	float mu = .19903788;	//398600.4418 for earth	  // .19903788 for correct period
	float small = .000001;
	float pi = 3.14159265358979;				  // pi to 15 digits
	float Re_cm = 63710000;

	// Function to find Position and Velocity
	UFUNCTION(BlueprintCallable)
	void Satellite_RV(float dt0, FVector r0, FVector v0);

	// finding c2 and c3
	void FindC2C3(float znew, float& c2new, float& c3new);
};
