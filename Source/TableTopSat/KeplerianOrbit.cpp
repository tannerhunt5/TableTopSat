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
	
	R_ijk.AddZeroed(1);
	V_ijk.AddZeroed(1);
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

TArray<FVector> AKeplerianOrbit::COE2RV(float p, float ecc, float incl, float RAAN, float argp, float nu, float mu)
{
	float incl_rad = FMath::DegreesToRadians(incl);
	float RAAN_rad = FMath::DegreesToRadians(RAAN);
	float argp_rad = FMath::DegreesToRadians(argp);

	if (ecc < small)
	{ 
		/* Circular Equatorial Condition*/
		if (incl_rad < small || abs(incl_rad - pi) < small)
		{
			
			argp_rad = 0.0f;
			RAAN_rad = 0.0f;
			//nu = truelon;
		}
		else
		{
		/* Circular inclined Condition */
			argp_rad = 0.0;
			//nu = arglat;
		}
	
	}
	else
	{
		/* Elliptical Equatorial Condition */
		if (incl_rad < small || abs(incl_rad - pi) < small)
		{
			//argp = lonper;
			RAAN_rad = 0.0;
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

	vpqw.X = -sinnu * sqrt((mu*TimeMultiplier)/p);
	vpqw.Y = (ecc + cosnu)*sqrt((mu*TimeMultiplier)/p);
	vpqw.Z = 0.0;


	// Performing transformation to IJK //
	FVector tempvec = rot3(rpqw, -argp_rad);
	tempvec = rot1(tempvec, -incl_rad);
	FVector r = rot3(tempvec, -RAAN_rad);

	// Checking r
	//UE_LOG(LogTemp, Warning, TEXT("r is %s"), *r.ToString());

	tempvec = rot3(vpqw, -argp_rad);
	tempvec = rot1(tempvec, -incl_rad);
	FVector v = rot3(tempvec, -RAAN_rad);

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


void AKeplerianOrbit::DrawOrbit()
{
	if (bUpdateOrbit)
	{
		TempState.Reset();
		R_ijk.Reset();
		V_ijk.Reset();

		mu = .19903788 * TimeMultiplier;

		// this is wrong, apogee =/= semi major axis (dumb mistake lol) will fix
		FindSemiLatusRectum((OrbitAltitudeKm*DistScale + 50), Eccentricity);

		int colorR = FMath::RandRange(0, 255);
		int colorG = FMath::RandRange(0, 255);
		int colorB = FMath::RandRange(0, 255);

		for (int i = 0; i < temp.Num(); i++)
		{

			TempState = COE2RV(p, Eccentricity, Inclination, RAAN, ArgOfPeriapsis, temp[i], mu);//  10000.0f,0.2f,1.3f,0.0f,0.0f,0.0f

			R_ijk.Insert(TempState[0], i);
			V_ijk.Insert({ -TempState[1].X, -TempState[1].Y ,-TempState[1].Z }, i);
		}

		if (bDoDraw)
		{
			for (int i = 0; i < R_ijk.Num(); i++)
			{

				if (i != R_ijk.Num() - 1)
				{
					DrawDebugLine(
						GetWorld(),
						R_ijk[i],
						R_ijk[i + 1],
						FColor(colorR, colorG, colorB),
						false, 60, 0,
						.15
					);

				}
				else
				{
					DrawDebugLine(
						GetWorld(),
						R_ijk[0],
						R_ijk[i],
						FColor(colorR, colorG, colorB),
						false, 60, 0,
						.15
					);

					break;
				}

			}
		}

		UE_LOG(LogTemp, Warning, TEXT("R_ijk in KeplerianOrbit is %s"), *R_ijk[0].ToString());
		UE_LOG(LogTemp, Warning, TEXT("V_ijk in KeplerianOrbit is %s"), *V_ijk[0].ToString());

		bUpdateOrbit = false;

		Period = 2 * pi*std::sqrt(std::pow((OrbitAltitudeKm*DistScale + 50), 3) / (mu*TimeMultiplier));
		UE_LOG(LogTemp, Warning, TEXT("Period = %f"), Period); // sidereal period = 86164
	}
}