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
	
	// Set initial location and velocity
}

// Called every frame
void AKeplerProblem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	Kepler(DeltaTime, r_init, v_init);
	// For first tick, call Kepler() using DeltaSeconds and initial location and velocity

	// Set new position based on result from first tick

	// Reassign new inputs to Kepler() arguments as new location and velocity
}



void AKeplerProblem::Kepler(float dt0, FVector r0, FVector v0)
{
	// Local variables
	FVector hbar;
	float p, s, w, a, xold, xnew, xnewsqrd, halfpi, rdotv, dt, dtnew, alpha, sme,
		magro, magvo, magh, period, temp, c2new, c3new, rval, f, g, magr, magv, 
		fdot, gdot, znew;
	int ktr, i, numiter, mulrev;

	// Initialize values
	ktr = 0;
	xold = 0;
	halfpi = pi * .5;
	xnew = 0;
	mulrev = 0;
	dt = dt0;
	numiter = 50;
	i = 0;
	rdotv = 0;
	

	if (abs(dt0) > small)
	{
		UE_LOG(LogTemp, Warning, TEXT("abs(dt0) > small"));
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
				xold = sqrt(mu)*(dt)*alpha;
			}
			else
			{
				xold = sqrt(mu)*(dt)*alpha*.97;
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

				xold = sqrt(p) * (2 * (cos(2 * w) / sin(2 * w)));
				alpha = 0;
			}
			else
			{
				// Hyperbola
				temp = -2.0 * mu * dt /
					(a * (rdotv + FMath::Sign(dt) * sqrt(-mu * a) *	(1.0 - magro * alpha)));
				xold = FMath::Sign(dt) * sqrt(-a) * log(temp);
			}
		}

		ktr = 1;
		dtnew = -10.0;
		// conv for dtsec to x units
		double tmp = 1.0 / sqrt(mu);

		while (abs(dtnew * temp - dt) >= small && ktr < numiter)
		{
			//UE_LOG(LogTemp, Warning, TEXT("ktr = %i"), ktr);
			znew = pow(xold, 2) * alpha;

			// Find c2 and c3
			//c2new = FindC2(znew);
			//c3new = FindC3(znew);
			FindC2C3(znew, c2new, c3new);

			// using newton iteration for new values 
			rval = pow(xold, 2) * c2new + rdotv * tmp * xold * (1 - znew * c3new) + magro * (1 - znew * c2new);
			dtnew = pow(xold, 2) * xold * c3new + rdotv * tmp * pow(xold, 2) *c2new + magro * xold * (1 - znew * c3new); 

			// Find new X
			xnew = xold + (dt * sqrt(mu) - dtnew) / rval;

			// if universal param goes negative, use bisection
			if (xnew < 0)
			{
				xnew = xold * .5;
			}

			ktr++;
			xold = xnew;
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

			for (i = 0; i < 3; i++)
			{
				r_ijk[i] = f * r0[i] + g * v0[i];
			}

			magr = r_ijk.Size();
			
			// gdot and fdot equations
			fdot = (sqrt(mu) *xnew / (magro*magr))*(znew*c3new - 1);
			gdot = 1 - (xnewsqrd * c2new / magr); 

			for (i = 0; i < 3; i++)
			{
				v_ijk[i] = fdot * r0[i] + gdot * v0[i];
			}

			magv = v_ijk.Size();
			temp = f * gdot - fdot * g;

			if (abs(temp - 1) > small)
			{
				UE_LOG(LogTemp, Warning, TEXT("f and g result: %d"), temp);
			}
		}

	}
	else
	{
		// if dt zero position remains the same
		r_ijk = r0;
		v_ijk = v0;
	}

}


void AKeplerProblem::FindC2C3(float znew, float & c2new, float & c3new)
{
	if (znew > small)
	{
		c2new = (1 - cos(sqrt(znew))) / znew;
		c3new = (sqrt(znew) - sin(sqrt(znew))) / pow(sqrt(znew), 3);
	}
	else
	{
		if (znew < -small)
		{
			c2new = (1 - cosh(sqrt(-znew))) / znew;
			c3new = (sinh(sqrt(-znew)) - sqrt(-znew)) / pow(sqrt(-znew), 3);
		}
		else
		{
			c2new = 0.5;
			c3new = 1.0 / 6.0;
		}
	}
}

//float AKeplerProblem::FindC2(float Psi_temp)
//{
//	float C2new;
//
//	if (Psi_temp > small)
//	{
//		C2new = (1 - cos(sqrt(Psi_temp))) / (Psi);
//	}
//	else 
//	{	
//		if (Psi_temp > - small)
//		{ 
//			C2new = (1 - cosh(sqrt(-Psi_temp))) / (Psi);
//		}
//		else
//		{
//			C2new = 1 / 2;
//		}
//	}
//
//	return C2new;
//}
//
//float AKeplerProblem::FindC3(float Psi_Temp)
//{
//	float C3new, sqrtPsi;
//	
//	if (Psi_Temp > small)
//	{
//		sqrtPsi = sqrt(Psi_Temp);
//		C3new = (sqrtPsi - sin(sqrtPsi)) / sqrt(pow(Psi_Temp,3));
//	}
//	else
//	{
//		if (Psi_Temp > -small)
//		{
//			sqrtPsi = sqrt(-Psi_Temp);
//			C3new = (sinh(sqrtPsi) - sqrtPsi) / (sqrt(pow(-Psi_Temp,3)));
//		}
//		else
//		{
//			C3new = 1 / 6;
//		}
//	}
//	return C3new;
//}


