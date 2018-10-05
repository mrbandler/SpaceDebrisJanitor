// Copyright (c) 2018 fivefingergames.

#include "Orbiter.h"
#include "OrbitalMechanics/Actors/Planet.h"
#include "OrbitalMechanics/Components/OrbitalMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Math/NumericLimits.h"
#include "EngineUtils.h"
#include "Engine.h"

AOrbiter::AOrbiter()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(false);
	SetRootComponent(StaticMeshComponent);

	OrbitalMovementComponent = CreateDefaultSubobject<UOrbitalMovementComponent>(TEXT("OrbitalMovementComponent"));
}

void AOrbiter::BeginPlay()
{
	Super::BeginPlay();
}

void AOrbiter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}