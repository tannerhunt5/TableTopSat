// Fill out your copyright notice in the Description page of Project Settings.

#include "KeplerianOrbit.h"
#include "Engine.h"
#include <cmath>
#include <math.h> 

// Sets default values
AKeplerianOrbit::AKeplerianOrbit()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AKeplerianOrbit::BeginPlay()
{
	Super::BeginPlay();

	//FindSemiLatusRectum(SemiMajorAxis, Eccentricity);
	//
	//TArray<float> temp = CreateNuArray();
	temp = CreateNuArray();

	EccentricityTemp = Eccentricity;
	SemiMajorAxisTemp = SemiMajorAxis;
	InclinationTemp = Inclination;
	RAANTemp = RAAN;
	ArgOfPeriapsisTemp = ArgOfPeriapsis;
	NumberOfPointsTemp = NumberOfPoints;

	Period = 2 * pi*std::sqrt(std::pow(SemiMajorAxis, 3) / mu);
	UE_LOG(LogTemp, Warning, TEXT("Period = %f"), Period);
	TwoPlusTwo();
	
	
}

// Called every frame
void AKeplerianOrbit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DrawOrbit();
}

float AKeplerianOrbit::FindSemiLatusRectum(float a, float e)
{
	if (e == 1)
	{
		p = a;
	}
	else
	{
		p = a * (1 - pow(e,2));
	}
	
	//UE_LOG(LogTemp, Warning, TEXT("semilat is %f"), p);

	return p;
}

TArray<FVector> AKeplerianOrbit::COE2RV(float p, float ecc, float incl, float omega, float argp, float nu)
{
	float incl_rad = FMath::DegreesToRadians(incl);

	if (ecc < small)
	{ 
		/* Circular Equatorial Condition*/
		if (incl_rad < small || abs(incl_rad - pi) < small)
		{
			
			argp = 0.0f;
			omega = 0.0f;
			//nu = truelon;
		}
		else
		{
		/* Circular inclined Condition */
			argp = 0.0;
			//nu = arglat;
		}
	
	}
	else
	{
		/* Elliptical Equatorial Condition */
		if (incl_rad < small || abs(incl_rad - pi) < small)
		{
			//argp = lonper;
			omega = 0.0;
		}
	}

	// Forming pqw position and velocity vectors //
	FVector rpqw;
	FVector vpqw;
	float cosnu = cos(nu);
	float sinnu = sin(nu);
	float temp = p / (1.0 + ecc * cosnu);

	rpqw.X = temp * cosnu;
	rpqw.Y = temp * sinnu;
	rpqw.Z = 0.0f;

	// Checking cosnu
	//UE_LOG(LogTemp, Warning, TEXT("semilat is %f"), p);

	if (abs(p) < 0.0001)
	{
		p = 0.0001;
	}

	vpqw.X = -sinnu * sqrt(mu/p);
	vpqw.Y = (ecc + cosnu)*sqrt(mu/p);
	vpqw.Z = 0.0;


	// Performing transformation to IJK //
	FVector tempvec = rot3(rpqw, -argp);
	tempvec = rot1(tempvec, -incl_rad);
	FVector r = rot3(tempvec, -omega);

	// Checking r
	//UE_LOG(LogTemp, Warning, TEXT("r is %s"), *r.ToString());

	tempvec = rot3(vpqw, -argp);
	tempvec = rot1(tempvec, -incl_rad);
	FVector v = rot3(tempvec, -omega);

	// Checking v
	//UE_LOG(LogTemp, Warning, TEXT("r is %s"), *r.ToString());

	TArray<FVector> RV;

	RV.Add(r);
	RV.Add(v);

	return RV;
}

TArray<float> AKeplerianOrbit::CreateNuArray()
{
	float temp = 0;

	for (int i = 0; i < NumberOfPoints; i++)
	{
		temp += 2 * pi / NumberOfPoints;
		nu.Add(temp);
		
		//UE_LOG(LogTemp, Warning, TEXT("Array of True Anomaly's: %f"), nu[i])
	}

	return nu;
}

FVector AKeplerianOrbit::rot1(FVector vec, float xval)
{
	float c, s, temp;
	temp = vec.Z;
	c = cos(xval);
	s = sin(xval);

	outvec1.Z = c * vec.Z - s * vec.Y;
	outvec1.Y= c * vec.Y + s * temp;
	outvec1.X = vec.X;
	
	return outvec1;
}

FVector AKeplerianOrbit::rot3(FVector vec, float xval)
{
	float c, s, temp;
	temp = vec.Y;
	c = cos(xval);
	s = sin(xval);


	outvec3.Y = c * vec.Y - s * vec.X;
	outvec3.X = c * vec.X + s * temp;
	outvec3.Z = vec.Z;


	return outvec3;
}

float AKeplerianOrbit::TwoPlusTwo()
{
	float AdditionResult = 4.0f;
	return AdditionResult;
}

void AKeplerianOrbit::DrawOrbit()
{
	if (UpdateOrbit)
	{
		TempState.Reset();
		R_ijk.Reset();
		V_ijk.Reset();

		FindSemiLatusRectum(SemiMajorAxis, Eccentricity);

		int colorR = FMath::RandRange(0, 255);
		int colorG = FMath::RandRange(0, 255);
		int colorB = FMath::RandRange(0, 255);

		for (int i = 0; i < temp.Num(); i++)
		{

			TempState = COE2RV(p, Eccentricity, Inclination, RAAN, ArgOfPeriapsis, temp[i]);//  10000.0f,0.2f,1.3f,0.0f,0.0f,0.0f

			R_ijk.Insert(TempState[0], i);
			V_ijk.Insert(TempState[1], i);
		}

		for (int i = 0; i < R_ijk.Num(); i++)
		{

			if (i != R_ijk.Num() - 1)
			{
				DrawDebugLine(
					GetWorld(),
					R_ijk[i],
					R_ijk[i + 1],
					FColor(colorR, colorG, colorB),
					false, 100, 0,
					1
				);

			}
			else
			{
				DrawDebugLine(
					GetWorld(),
					R_ijk[0],
					R_ijk[i],
					FColor(colorR, colorG, colorB),
					false, 100, 0,
					1
				);


				break;
			}


		}

		EccentricityTemp = Eccentricity;
		SemiMajorAxisTemp = SemiMajorAxis;
		InclinationTemp = Inclination;
		RAANTemp = RAAN;
		ArgOfPeriapsisTemp = ArgOfPeriapsis;
		NumberOfPointsTemp = NumberOfPoints;

		UE_LOG(LogTemp, Warning, TEXT("R_ijk in KeplerianOrbit is %s"), *R_ijk[0].ToString());
		UE_LOG(LogTemp, Warning, TEXT("V_ijk in KeplerianOrbit is %s"), *V_ijk[0].ToString());

		UpdateOrbit = false;
	}
}