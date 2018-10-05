// Copyright (c) 2018 fivefingergames.

#include "Planet.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

APlanet::APlanet()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetSimulatePhysics(true);
	StaticMeshComponent->SetEnableGravity(false);
}

void APlanet::BeginPlay()
{
	Super::BeginPlay();
}

void APlanet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float APlanet::GetMass() const
{
	return StaticMeshComponent->GetMass();
}

FVector APlanet::GetCenter() const
{
	return GetActorLocation();
}

float APlanet::GetRadius() const
{
	return StaticMeshComponent->Bounds.GetSphere().W;
}