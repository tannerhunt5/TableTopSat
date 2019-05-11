// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

	// Initial r0 and v0 for ISS
	FVector r_init = { 100, 100, 100 };
	FVector v_init = { 10, 10, 0 };

	FVector r_ijk0;
	FVector v_ijk0;

	float Zeta;
	float Alpha;
	float Xi_0;
	float Psi;
	float Xi_n;
	float Xi_np1;
	float Cot2w;
	float r;


	float FindZeta(FVector r0, FVector v0);
	//float FindAlpha(FVector r0, FVector v0);

	void FindXi0(float dt, FVector r0, FVector v0);
	float FindPsi(float dt, FVector r0, FVector v0);
	float FindC2(float Psi_temp);
	float FindC3(float Psi_Temp);
};
