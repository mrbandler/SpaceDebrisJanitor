// Copyright (c) 2018 fivefingergames.

#include "DebrisSpawnerComponent.h"
#include "OrbitalMechanics/Actors/Planet.h"
#include "OrbitalMechanics/Actors/Orbiter.h"
#include "OrbitalMechanics/Components/OrbitalMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Containers/Array.h"
#include "Engine/World.h"

DEFINE_LOG_CATEGORY(LogDebrisSpawnerComponent);

UDebrisSpawnerComponent::UDebrisSpawnerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDebrisSpawnerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UDebrisSpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UDebrisSpawnerComponent::SpawnDebris()
{
	if (PlanetForDebris == nullptr)
	{
		UE_LOG(LogDebrisSpawnerComponent, Warning, TEXT("Could not spawn debris, because no planet was specified."));
		
		return;
	}

	TArray<FVector> locations = GenerateLocations();

	for (int i = 0; i < locations.Num(); i++) 
	{
		FVector location = locations[i];
		FVector initialDirection = GetInitialDirection(location);

		FActorSpawnParameters SpawnParams;
		AOrbiter* debris = GetWorld()->SpawnActor<AOrbiter>(DebrisToSpawn, location, FRotator::ZeroRotator, SpawnParams);
		if (debris != nullptr) 
		{
			if (SpawnDebrisWithDebuggingEnabled == true)
			{
				debris->OrbitalMovementComponent->VisualDebugging = true;
				debris->OrbitalMovementComponent->LineTraceLength = 2.0f;
				debris->OrbitalMovementComponent->LineThickness = 80.0f;
				debris->OrbitalMovementComponent->PermanentLines = false;
				debris->OrbitalMovementComponent->InitialDirection = initialDirection;
			}
		}
	}
}

TArray<FVector> UDebrisSpawnerComponent::GenerateLocations()
{
	TArray<FVector> result;

	float planetSize = PlanetForDebris->GetRadius() * 2.0f;

	int32 heightMin = planetSize * SpawnHeightMinPercent;
	heightMin = planetSize + heightMin;

	int32 heigthMax = planetSize * SpawnHeightMaxPercent;
	heigthMax = planetSize + heigthMax;

	for (int i = 0; i < DebrisCount; i++)
	{
		float heigth = FMath::RandRange(heightMin, heigthMax);
		FVector location = FMath::VRand() * heigth;
		result.Add(location);
	}

	return result;
}

FVector UDebrisSpawnerComponent::GetInitialDirection(FVector InitialLocation)
{
	FVector direction;
	int32 rand = FMath::RandRange(0, 1);
	if (rand == 0) 
	{
		direction = -FVector::UpVector;
	}
	else 
	{
		direction = FVector::UpVector;
	}

	FVector calcDirection = PlanetForDebris->GetCenter() - InitialLocation;
	FVector result = FVector::CrossProduct(calcDirection, direction);
	result.Normalize();

	return result;
}

