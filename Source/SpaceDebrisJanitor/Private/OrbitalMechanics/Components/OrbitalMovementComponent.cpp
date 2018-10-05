// Copyright (c) 2018 fivefingergames.

#include "OrbitalMovementComponent.h"
#include "Planet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Math/NumericLimits.h"
#include "Kismet/KismetMathLibrary.h"
#include "EngineUtils.h"
#include "Engine.h"

DEFINE_LOG_CATEGORY(LogOrbitalMovementComponent);

UOrbitalMovementComponent::UOrbitalMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UOrbitalMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	StaticMeshComponent = GetStaticMeshComponentFromOwner();
	if (StaticMeshComponent != nullptr) 
	{
		FindNearestPlanetToOrbit();
	}
	else 
	{
		UE_LOG(LogOrbitalMovementComponent, Fatal, TEXT("Static Mesh Component could not be found on owning actor. Unable to perform orbital movement!"));
	}
}

void UOrbitalMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlanetToOrbit != nullptr) 
	{
		if (StaticMeshComponent->ComponentVelocity.IsZero() == true) 
		{
			StaticMeshComponent->ComponentVelocity += InitialDirection * CalculateInitialVelocity();
		}
		
		CalculateOrbit(DeltaTime);
	}
}

APlanet* UOrbitalMovementComponent::GetOrbitingPlanet() const
{
	return PlanetToOrbit;
}

void UOrbitalMovementComponent::CalculateOrbit(float DeltaSeconds)
{
	m_FixedUpdateCounter += DeltaSeconds;
	if (m_FixedUpdateCounter >= FixedUpdateTimestep) {
		m_FixedUpdateCounter = 0.0f;

		FVector gravity = CalculateGravity();

		FVector velocity = StaticMeshComponent->ComponentVelocity + gravity * DeltaSeconds;
		FVector location = GetOwner()->GetActorLocation();
		FVector newLocation = location + (StaticMeshComponent->ComponentVelocity + velocity);

		StaticMeshComponent->ComponentVelocity = velocity;

		FHitResult hitResult;
		StaticMeshComponent->SetWorldLocation(newLocation, true, &hitResult);
		if (&hitResult)
		{
			if (hitResult.GetActor() == PlanetToOrbit)
			{
				GetOwner()->Destroy();
			}
		}

		DrawVisualDebugging(location, newLocation, gravity, StaticMeshComponent->ComponentVelocity);
	}
}

FVector UOrbitalMovementComponent::CalculateGravity() const
{
	FVector direction = PlanetToOrbit->GetCenter() - GetOwner()->GetActorLocation();
	float squareMagnitude = direction.SizeSquared();
	direction.Normalize();

	return direction * ((GravitationalConstant * PlanetToOrbit->GetMass()) / squareMagnitude);
}

float UOrbitalMovementComponent::CalculateInitialVelocity() const
{
	float distance = FVector::Distance(GetOwner()->GetActorLocation(), PlanetToOrbit->GetCenter());
	float velocity = FMath::Sqrt((GravitationalConstant * PlanetToOrbit->GetMass()) / distance) / 15.0f;
	float percentEllipseVelocity = velocity * PercentEllipse;
	return velocity + percentEllipseVelocity;
}

float UOrbitalMovementComponent::CalculateEscapeVelocity() const
{
	float distance = FVector::Distance(GetOwner()->GetActorLocation(), PlanetToOrbit->GetCenter());
	return FMath::Sqrt((2.0f * GravitationalConstant * PlanetToOrbit->GetMass()) / distance) / 15.0f;
}

float UOrbitalMovementComponent::GetMass() const
{
	return StaticMeshComponent->GetMass();
}

void UOrbitalMovementComponent::DrawVisualDebugging(const FVector& LastLocation, const FVector& CurrentLocation, const FVector& Gravity, const FVector& Velocity)
{
	if (VisualDebugging == true)
	{
		if (PermanentLines == true) {
			m_DebugLinesDrawn = true;
		}
		else if (PermanentLines == false && m_DebugLinesDrawn == true)
		{
			m_DebugLinesDrawn = false;
			GetWorld()->Exec(GetWorld(), TEXT("flushpersistentdebuglines"));
		}

		DrawDebugLine(GetWorld(), LastLocation, CurrentLocation, FColor::Red, PermanentLines, LineTraceLength, 0, LineThickness);

		//FString gravityMessage = "Gravity: " + Gravity.ToString();
		//FString velocityMessage = "Velocity: " + Velocity.ToString();
		//FString locationMessage = "Location: " + GetOwner()->GetActorLocation().ToString();
		//GEngine->AddOnScreenDebugMessage(0, 0.5f, FColor::Red, gravityMessage);
		//GEngine->AddOnScreenDebugMessage(1, 0.5f, FColor::Red, velocityMessage);
		//GEngine->AddOnScreenDebugMessage(2, 0.5f, FColor::Red, locationMessage);
	}
	else
	{
		if (m_DebugLinesDrawn == true) {
			m_DebugLinesDrawn = false;
			GetWorld()->Exec(GetWorld(), TEXT("flushpersistentdebuglines"));
		}
	}
}

UStaticMeshComponent* UOrbitalMovementComponent::GetStaticMeshComponentFromOwner()
{
	return Cast<UStaticMeshComponent>(GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass()));
}

void UOrbitalMovementComponent::FindNearestPlanetToOrbit()
{
	UWorld* world = GetWorld();
	if (world != nullptr) {
		float smallesDistance = TNumericLimits<int32>::Max();
		for (TActorIterator<APlanet> PlanetIterator(world); PlanetIterator; ++PlanetIterator)
		{
			APlanet* planet = *PlanetIterator;
			float distance = FVector::Distance(planet->GetCenter(), GetOwner()->GetActorLocation());
			if (distance < smallesDistance) {
				smallesDistance = distance;
				PlanetToOrbit = planet;
			}
		}
	}
}
