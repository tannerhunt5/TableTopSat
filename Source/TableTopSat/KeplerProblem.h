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
	//float mu = .19903788;	//398600.4418;					  // .19903788 for correct period
	float small = .000001;
	float pi = 3.14159265358979;				  // pi to 15 digits
	float Re_cm = 63710000;                       // Radius Earth centimeters
	int NumFrame = 0;


	// Initial r0 and v0 for ISS
	FVector r_init = { 52.593, 2.719, 3.480 };//-5.753, -61.623, 3.668 
	FVector v_init = { -7.272,  53.316, 68.241 };//0.005, 0.038, -0.048//-.162, 1.91, 1.525   // X=56.862 Y=-1.945 Z=56.498

	FVector r_current;
	FVector v_current;

	UFUNCTION(BlueprintCallable)
	FVector Kepler(float dt0, FVector r0, FVector v0);

	void FindC2C3(float znew, float& c2new, float& c3new);

	UPROPERTY(EditAnywhere)
	AKeplerianOrbit* OrbitPtr;

	bool accessed;

	UPROPERTY(EditAnywhere)
	AKeplerianOrbit* OrbitPtr;

	bool accessed;

};
