// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Planet.generated.h"

UCLASS()
class SPACEDEBRISJANITOR_API APlanet : public AActor
{
	GENERATED_BODY()
	
public:	
	APlanet();

	UPROPERTY(VisibleAnywhere)
	class UStaticMeshComponent* StaticMeshComponent;

	float GetMass() const;
	FVector GetCenter() const;
	float GetRadius() const;

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;	
	
};
