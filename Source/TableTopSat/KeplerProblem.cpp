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

	//int x = 0;
	//while (x < 5)
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("x is %d"), x);
	//	x++;
	//}
	//UE_LOG(LogTemp, Warning, TEXT("Tick Executed"));


}

float AKeplerProblem::FindZeta(FVector r0, FVector v0)
{
	return (pow(v0.Size(),2) / 2) - (mu / r0.Size());
}


void AKeplerProblem::FindXi0(float dt, FVector r0, FVector v0)
{
	// Local variables
	FVector hbar;
	float p, s, w, a;


	// Values From other function calls
	Alpha = (-pow(v0.Size(), 2) / mu) + (2 / r0.Size());


	// Implementation
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

		s = 0.5 * (pi/2) - atan(3 * sqrt(mu / pow(p, 3))*dt); 

		w = atan(pow(tan(s), 1/3 ));

		Xi_0 = sqrt(p) * ( 2 * (cos(2*w)/sin(2*w)) );

	}
	else if (Alpha < -small)
	{
		// Hyperbolaa
		a = 1 / Alpha;

		Xi_0 = FMath::Sign(dt) * sqrt(-a)*log((-2 * mu*Alpha*dt) / (FVector::DotProduct(r0, v0) + (-dt)*sqrt(-mu * a)*(1 - r0.Size()*a)));
	}

	// Find Psi
}

float AKeplerProblem::FindPsi(float dt, FVector r0, FVector v0)
{

	float Xi_n_old = Xi_0;
	while (abs(Xi_n_old - Xi_np1) < small)
	{
		Psi = pow(Xi_n, 2)*Alpha;

		r = pow(Xi_n, 2)*FindC2(Psi) + FVector::DotProduct(r0, v0)/sqrt(mu)*Xi_n*(1-Psi*FindC3(Psi))+r0.Size()*(1-Psi*FindC2(Psi));

		float num = sqrt(mu)*dt - pow(Xi_n, 3)*FindC3(Psi) - FVector::DotProduct(r0, v0) / sqrt(mu)*pow(Xi_n, 2)*FindC2(Psi) - r0.Size()*Xi_n*(1 - Psi * FindC3(Psi));
		Xi_np1 = Xi_n + num/r0.Size();

		
		Xi_n = Xi_np1;
	}

	return Psi;
}

float AKeplerProblem::FindC2(float Psi_temp)
{
	float C2new;

	if (Psi_temp > small)
	{
		C2new = (1 - cos(sqrt(Psi_temp))) / (Psi);
	}
	else 
	{	
		if (Psi_temp > - small)
		{ 
			C2new = (1 - cosh(sqrt(-Psi_temp))) / (Psi);
		}
		else
		{
			C2new = 1 / 2;
		}
	}

	return C2new;
}

float AKeplerProblem::FindC3(float Psi_Temp)
{
	float C3new, sqrtPsi;
	
	if (Psi_Temp > small)
	{
		sqrtPsi = sqrt(Psi_Temp);
		C3new = (sqrtPsi - sin(sqrtPsi)) / sqrt(pow(Psi_Temp,3));
	}
	else
	{
		if (Psi_Temp > -small)
		{
			sqrtPsi = sqrt(-Psi_Temp);
			C3new = (sinh(sqrtPsi) - sqrtPsi) / (sqrt(pow(-Psi_Temp,3)));
		}
		else
		{
			C3new = 1 / 6;
		}
	}
	return C3new;
}


