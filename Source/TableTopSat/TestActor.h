// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KeplerianOrbit.h"
#include "Engine.h"
#include "TestActor.generated.h"

UCLASS()
class TABLETOPSAT_API ATestActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATestActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UPROPERTY(EditAnywhere)
	AKeplerianOrbit* OrbitPtr;

	TArray<FVector> rtemp;
	TArray<FVector> vtemp;
	FVector rvectemp;

	bool accessed = false;
	bool IsUpdateOrbitTrue;

	FVector InitialLocation;
	FVector OrbitLocation;
	FVector NewOrbitLocation;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	UStaticMeshComponent* SM;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
	USceneComponent* Root;

	// Function to Find closest point from Current location to points along orbit
	int GetClosestPoint();
	TArray<float> Distances;
	int MinIndex = 0;
	float minimum;

	// New rotation code will be tested here
	UPROPERTY(EditAnywhere, Category = "Movement")
	float AngleAxis;
	
	// This value is the distance away from player?
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector Dimensions = {300,0,0};

	// What axis should be pointing towards origin
	UPROPERTY(EditAnywhere, Category = "Movement")
	FVector AxisVector = {0,0,1};

	// Smooth movement?
	UPROPERTY(EditAnywhere, Category = "Movement")
	float Multiplier = 50;

	void Revolve();

};
