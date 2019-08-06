// Fill out your copyright notice in the Description page of Project Settings.

#include "Satellite.h"
#include "Engine.h"

// TODO implement SGP4 model to find RV from TLE and a specific time. Separate class maybe?

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
	
	rsat_current = InitialPosition;
	vsat_current = InitialVelocity;

	SetActorRotation(InitRotation);
	
}

// Called every frame
void ASatellite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GetLVLHRot();

	Satellite_RV(DeltaTime, rsat_current, vsat_current);
}

void ASatellite::Satellite_RV(float dt0, FVector r0, FVector v0)
{
	//FVector  rsat_current, vsat_current;


	// Local variables
	FVector hbar, r_ijk, v_ijk;
	float p, s, w, a, xold, xnew, xnewsqrd, halfpi, rdotv, dt, dtnew, alpha, sme,
		magro, magvo, magh, period, temp, c2new, c3new, rval, f, g, magr, magv,
		fdot, gdot, znew;
	int ktr, i, numiter;

	// Initialize values
	ktr = 0;
	xold = 0;
	halfpi = pi * .5;
	xnew = 0; znew = 0;
	dt = dt0;
	numiter = 50;
	i = 0;
	rdotv = 0;
	c2new = 0; c3new = 0;



	if (abs(dt0) > small)
	{
		//UE_LOG(LogTemp, Warning, TEXT("abs(dt0) > small"));
		magro = r0.Size();
		magvo = v0.Size();
		rdotv = FVector::DotProduct(r0, v0);

		//UE_LOG(LogTemp, Warning, TEXT("mu = %f"), mu);

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
		dtnew = -2.0;
		// conv for dtsec to x units
		double tmp = 1.0 / sqrt(mu);
		//UE_LOG(LogTemp, Warning, TEXT("dtnew at checkpoint 1 = %f"), dtnew);

		while (abs(dtnew * tmp - dt) >= small && ktr < numiter)
		{
			//UE_LOG(LogTemp, Warning, TEXT("dtnew at checkpoint 2 = %f"), dtnew);
			znew = pow(xold, 2) * alpha;

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

			//UE_LOG(LogTemp, Error, TEXT("dtnew at checkpoint 3 = %f"), dtnew);
			ktr++;
			xold = xnew;
		}

		if (ktr >= numiter)
		{
			UE_LOG(LogTemp, Error, TEXT("Not converged in %i iterations"), numiter);

		}
		else // Find position and velocity vectors at new time
		{

			xnewsqrd = pow(xnew, 2);

			// f and g solutions
			f = 1 - (xnewsqrd * c2new / magro);
			g = dt - xnewsqrd * xnew * c3new / sqrt(mu);

			// Calculating new r_ijk
			r_ijk.X = f * r0.X + g * v0.X;
			r_ijk.Y = f * r0.Y + g * v0.Y;
			r_ijk.Z = f * r0.Z + g * v0.Z;


			magr = r_ijk.Size();

			// gdot and fdot equations
			fdot = (sqrt(mu) *xnew / (magro*magr))*(znew*c3new - 1);
			gdot = 1 - (xnewsqrd * c2new / magr);

			v_ijk.X = fdot * r0.X + gdot * v0.X;
			v_ijk.Y = fdot * r0.Y + gdot * v0.Y;
			v_ijk.Z = fdot * r0.Z + gdot * v0.Z;

			magv = v_ijk.Size();

			temp = f * gdot - fdot * g;

			if (abs(temp - 1) > small)
			{
				UE_LOG(LogTemp, Warning, TEXT("f and g result: %f"), temp);
			}
		}

	}
	else
	{
		// if dt zero position remains the same
		r_ijk = r0;
		v_ijk = v0;
	}

	rsat_current = r_ijk;
	vsat_current = v_ijk;

	SetActorLocation(rsat_current); 
	
	DrawDebugLine(
		GetWorld(),
		{0,0,0},
		rsat_current,
		FColor(255, 0, 0),
		false, .1, 0,
		.1
	);

	Altitude = rsat_current.Size();
	//UE_LOG(LogTemp, Warning, TEXT("Altitude: %f"), Altitude);

}

void ASatellite::FindC2C3(float znew, float & c2new, float & c3new)
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

void ASatellite::GetLVLHRot()
{
	// Vector pointing towards origin
	FVector ToOrigin = GetActorLocation().GetSafeNormal();

	float DotProd = FVector::DotProduct(ToOrigin, InitialPosition.GetSafeNormal());
	float RotAng = acos(DotProd);

	FRotator NewRotation = FRotator(0, 0, -RotAng) + InitRotation;

	FQuat QuatRotation = FQuat(NewRotation);
	//UE_LOG(LogTemp, Warning, TEXT("Quaternion = %s"), *QuatRotation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Dot prod between ToOrigin and rsat_current = %f"), DotProd);

	float omega = mu / pow((Altitude + 50), 3);

	SetActorRotation(QuatRotation);
}

