// Fill out your copyright notice in the Description page of Project Settings.

#include "KeplerProblem.h"
#include <math.h>
#include "Runtime/Core/Public/Math/Vector.h"

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
void AKeplerProblem::RV_next(float dt, FVector r0, FVector v0)
{
	if (Alpha > small)
	{
		// Elliptical orbit
		Xi_0 = sqrt(mu)*(dt)*Alpha;
	}
	else if (abs(Alpha) < small)
	{
		// Parabola
		hbar = FVector::CrossProduct(r0, v0);

		p = pow(hbar.Size(), 2) / mu; 

		Cot2s = 3 * sqrt(mu / pow(p, 3))*dt; 

		Tan3w = 0.0;

		Xi_0 = sqrt(p) * 2 * Cot2w;

	}
	else if (Alpha < -small)
	{
		a = 1 / Alpha;
		Xi_0 = -dt * sqrt(-a)*log((-2 * mu*Alpha*dt) / (FVector::DotProduct(r0, v0) + (-dt)*sqrt(-mu * a)*(1 - r0.Size()*a)));
	}

}


