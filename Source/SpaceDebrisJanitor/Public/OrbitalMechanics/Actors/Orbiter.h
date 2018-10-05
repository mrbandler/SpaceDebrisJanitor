// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Orbiter.generated.h"

UCLASS()
class SPACEDEBRISJANITOR_API AOrbiter : public AActor
{
	GENERATED_BODY()
	
public:	
	AOrbiter();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UOrbitalMovementComponent* OrbitalMovementComponent;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	
};
