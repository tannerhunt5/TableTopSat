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


void AKeplerProblem::Kepler(float dt0, FVector r0, FVector v0)
{
	// Local variables
	FVector hbar;
	float p, s, w, a, xold, halfpi, Xi_new, rdotv, dt, dtnew, alpha, sme,
		magro, magvo, magh, period, temp, c2new, c3new, rval, xnew, xnewsqrd, f, g;
	int ktr, i, numiter, mulrev;

	// Initialize values
	ktr = 0;
	xold = 0;
	halfpi = pi * .5;
	Xi_new = 0;
	mulrev = 0;
	dt = dt0;
	numiter = 0;
	i = 0;
	rdotv = 0;
	

	// Values From other function calls
	Alpha = (-pow(v0.Size(), 2) / mu) + (2 / r0.Size());

	if (abs(dt0) > small)
	{
		magro = r0.Size();
		magvo = v0.Size();
		rdotv = FVector::DotProduct(r0, v0);

		// find sme, alpha, and a 
		sme = ((magvo * magvo) * 0.5) - (mu / magro);
		alpha = -sme * 2.0 / mu;

		if (abs(sme) > small)
		{
			a = -mu / (2.0 * sme);
		}
		else
		{
			a = pow(10, 1000);
			UE_LOG(LogTemp, Warning, TEXT("a is infinite"));
		}
		if (abs(alpha) < small)   // parabola
		{
			alpha = 0.0;
		}

		// Implementation
		if (alpha >= small)
		{
			period = 2 * pi*sqrt(abs(pow(a, 3)) / mu);
			// Elliptical orbit
			if (abs(alpha - 1) > small)
			{
				Xi_0 = sqrt(mu)*(dt)*alpha;
			}
			else
			{
				Xi_0 = sqrt(mu)*(dt)*alpha*.97;
			}
			
		}
		else
		{
			// Parabola
			if (abs(alpha) < small)
			{
				hbar = FVector::CrossProduct(r0, v0);
				magh = hbar.Size();

				p = pow(magh, 2) / mu;
				s = 0.5 * (pi / 2) - atan(3 * sqrt(mu / pow(p, 3))*dt);
				w = atan(pow(tan(s), 1 / 3));

				Xi_0 = sqrt(p) * (2 * (cos(2 * w) / sin(2 * w)));
				alpha = 0;
			}
			else
			{
				// Hyperbola
				temp = -2.0 * mu * dt /
					(a * (rdotv + FMath::Sign(dt) * sqrt(-mu * a) *	(1.0 - magro * alpha)));
				Xi_0 = FMath::Sign(dt) * sqrt(-a) * log(temp);
			}
		}

		ktr = 1;
		dtnew = -10.0;
		// conv for dtsec to x units
		double tmp = 1.0 / sqrt(mu);

		while (abs(dtnew * temp - dt) >= small && ktr < numiter)
		{
			Xi_new = pow(Xi_0, 2) * alpha;

			// Find c2 and c3
			c2new = FindC2(Xi_new);
			c3new = FindC3(Xi_new);

			// using newton iteration for new values 
			rval = pow(Xi_0, 2) * c2new + rdotv * tmp * Xi_0 * (1 - Xi_new * c3new) + magro * (1 - Xi_new * c2new);
			dtnew = pow(Xi_0, 2) * Xi_0 * c3new + rdotv * tmp * pow(Xi_0, 2) *c2new + magro * Xi_0 * (1 - Xi_new * c3new); 

			// Find new X
			xnew = Xi_0 + (dt * sqrt(mu) - dtnew) / rval;

			// if universal param goes negative, use bisection
			if (xnew < 0)
			{
				xnew = Xi_0 * .5;
			}

			ktr++;
			Xi_0 = xnew;
		}

		if (ktr >= numiter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Not converged in %i iterations"), numiter);

		}
		else
		{
			// Find position and velocity vectors at new time
			xnewsqrd = pow(xnew, 2);

			// f and g solutions
			f = 1 - (xnewsqrd *c2new / magro);
			g = dt - xnewsqrd * xnew*c3new / sqrt(mu); 

		}

	}

	
	//else if (abs(Alpha) < small)
	//{
	//	 Parabola
	//	hbar = FVector::CrossProduct(r0, v0);
	//	magh = hbar.Size();
	//	p = pow(hbar.Size(), 2) / mu; 
	//	s = 0.5 * (pi/2) - atan(3 * sqrt(mu / pow(p, 3))*dt); 
	//	w = atan(pow(tan(s), 1/3 ));
	//	Xi_0 = sqrt(p) * ( 2 * (cos(2*w)/sin(2*w)) );
	//}
	//else if (Alpha < -small)
	//{
	//	// Hyperbolaa
	//	a = 1 / Alpha;
	//	Xi_0 = FMath::Sign(dt) * sqrt(-a)*log((-2 * mu*Alpha*dt) / (rdotv + (-dt)*sqrt(-mu * a)*(1 - r0.Size()*a)));
	//}

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


