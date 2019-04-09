// Fill out your copyright notice in the Description page of Project Settings.

#include "Satellite.h"
#include "Engine.h"

// Sets default values
ASatellite::ASatellite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASatellite::BeginPlay()
{
	Super::BeginPlay();

	//FindSemiLatusRectum(SemiMajorAxis, Eccentricity);
	//
	//TArray<float> temp = CreateNuArray();
	temp = CreateNuArray();

	EccentricityTemp = Eccentricity;
	SemiMajorAxisTemp = SemiMajorAxis;
	InclinationTemp = Inclination;

	//TArray<FVector> FirstTempState;

	////R_ijk.SetNumZeroed(NumberOfPoints);
	////V_ijk.SetNumZeroed(NumberOfPoints);

	//for (int i = 0; i < temp.Num(); i++)
	//{

	//	TempState = COE2RV(p, Eccentricity, Inclination, RAAN, ArgOfPeriapsis,temp[i]);//  10000.0f,0.2f,1.3f,0.0f,0.0f,0.0f
	//	
	//	R_ijk.Insert(FirstTempState[0], i);
	//	V_ijk.Insert(FirstTempState[1], i);

	//}
	
	//for (int i = 0; i < R_ijk.Num(); i++)
	//{
	//	//UE_LOG(LogTemp, Warning, TEXT("Final Position Vector %s"), *R_ijk[i].ToString());

	//	//Orbit Points
	//	DrawDebugSphere(
	//		GetWorld(),
	//		R_ijk[i],
	//		10,
	//		5,
	//		FColor(0, 255, 0),
	//		true,
	//		1000
	//	);

	//	/*DrawDebugSphere(GetWorld(), R_ijk[i], 10.0f, 32, FColor(255, 0, 0), true, -1.0f);*/
	//}

	/*for (int i = 0; i < V_ijk.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Final Velocity Vector %s"), *V_ijk[i].ToString());
	}*/
	
	
}

// Called every frame
void ASatellite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FindSemiLatusRectum(SemiMajorAxis, Eccentricity);
	
	//for (int i = 0; i < temp.Num(); i++)
	//{

	//	TempState = COE2RV(p, Eccentricity, Inclination, RAAN, ArgOfPeriapsis,temp[i]);//  10000.0f,0.2f,1.3f,0.0f,0.0f,0.0f
	//	
	//	R_ijk.Insert(TempState[0], i);
	//	V_ijk.Insert(TempState[1], i);
	//}

	// github test


	if (Eccentricity != EccentricityTemp || SemiMajorAxis != SemiMajorAxisTemp || Inclination != InclinationTemp)
	{ 

		for (int i = 0; i < temp.Num(); i++)
		{

			TempState = COE2RV(p, Eccentricity, Inclination, RAAN, ArgOfPeriapsis, temp[i]);//  10000.0f,0.2f,1.3f,0.0f,0.0f,0.0f

			R_ijk.Insert(TempState[0], i);
			V_ijk.Insert(TempState[1], i);
		}

		for (int i = 0; i < R_ijk.Num(); i++)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Final Position Vector %s"), *R_ijk[i].ToString());

			//Orbit Points
			//DrawDebugSphere(
			//	GetWorld(),
			//	R_ijk[i],
			//	10,
			//	5,
			//	FColor(255, 0, 0),
			//	true,
			//	1000);
			
			if (i != R_ijk.Num()-1) 
			{
				DrawDebugLine(
					GetWorld(),
					R_ijk[i],
					R_ijk[i+1],
					FColor(255, 0, 0),
					false, -1, 0,
					12.333
				);

			}
			else { break; }
			
		}

		Eccentricity = EccentricityTemp;
		SemiMajorAxis = SemiMajorAxisTemp;
		Inclination = InclinationTemp;

	} 

	
}

float ASatellite::FindSemiLatusRectum(float a, float e)
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

TArray<FVector> ASatellite::COE2RV(float p, float ecc, float incl, float omega, float argp, float nu)
{

	if (ecc < small)
	{ 
		/* Circular Equatorial Condition*/
		if (incl < small || abs(incl - pi) < small)
		{
			argp = 0.0f;
			omega = 0.0f;
			// nu = truelon
		}
		else
		{
		/* Circular inclined Condition */
			argp = 0.0;
			// nu = arglat;
		}
	
	}
	else
	{
		/* Elliptical Equatorial Condition */
		if (incl < small || abs(incl - pi) < small)
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

	vpqw.X = -sinnu * sqrt(398600.4418) / sqrt(p);
	vpqw.Y = (ecc + cosnu)*sqrt(398600.4418) / sqrt(p);
	vpqw.Z = 0.0;


	// Performing transformation to IJK //
	FVector tempvec = rot3(rpqw, -argp);
	tempvec = rot1(tempvec, -incl);
	FVector r = rot3(tempvec, -omega);

	// Checking r
	//UE_LOG(LogTemp, Warning, TEXT("r is %s"), *r.ToString());

	tempvec = rot3(vpqw, -argp);
	tempvec = rot1(tempvec, -incl);
	FVector v = rot3(tempvec, -omega);

	// Checking v
	//UE_LOG(LogTemp, Warning, TEXT("r is %s"), *r.ToString());

	TArray<FVector> RV;

	RV.Add(r);
	RV.Add(v);

	return RV;
}

TArray<float> ASatellite::CreateNuArray()
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

FVector ASatellite::rot1(FVector vec, float xval)
{
	float temp = vec.Y;
	float c = cos(xval);
	float s = sin(xval);

	outvec1.Z = c * vec.Z - s * vec.Y;
	outvec1.Y= c * vec.Y + s * temp;
	outvec1.X = vec.X;
	
	return outvec1;
}

FVector ASatellite::rot3(FVector vec, float xval)
{
	float temp = vec.Y;
	float c = cos(xval);
	float s = sin(xval);


	outvec3.Y = c * vec.Y - s * vec.X;
	outvec3.X = c * vec.X + s * temp;
	outvec3.Z = vec.Z;


	return outvec3;
}

float ASatellite::TwoPlusTwo()
{
	float AdditionResult = 2.0f + 2.0f;
	return AdditionResult;
}

