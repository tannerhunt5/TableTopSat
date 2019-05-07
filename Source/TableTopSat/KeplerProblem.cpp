// Fill out your copyright notice in the Description page of Project Settings.

#include "KeplerProblem.h"
#include <math.h>

// Sets default values
AKeplerProblem::AKeplerProblem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeplerProblem::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKeplerProblem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AKeplerProblem::FindZeta(FVector r0, FVector v0)
{
	return (pow(v0.Size(),2) / 2) - (mu / r0.Size());
}

float AKeplerProblem::FindAlpha(FVector r0, FVector v0)
{
	return (-pow(v0.Size(),2)/mu) + (2/r0.Size());
} 

