// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitalMovementComponent.generated.h"

// Forward declarations.
class UStaticMeshComponent;
class APlanet;

DECLARE_LOG_CATEGORY_EXTERN(LogOrbitalMovementComponent, Log, All);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEDEBRISJANITOR_API UOrbitalMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UOrbitalMovementComponent();

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics")
	float FixedUpdateTimestep = 0.01f;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics")
	APlanet* PlanetToOrbit;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics")
	float GravitationalConstant = 667.4f;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PercentEllipse = 0.0f;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics")
	FVector InitialDirection = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics|Debugging")
	bool VisualDebugging = false;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics|Debugging")
	float LineTraceLength = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics|Debugging")
	float LineThickness = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Orbital Mechanics|Debugging")
	bool PermanentLines = false;

	APlanet* GetOrbitingPlanet() const;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UStaticMeshComponent* StaticMeshComponent;

	virtual void BeginPlay() override;
		
	float GetMass() const;

	FVector CalculateGravity() const;

	void CalculateOrbit(float DeltaSeconds);

	float CalculateInitialVelocity() const;
	float CalculateEscapeVelocity() const;

private:
	bool m_DebugLinesDrawn = false;
	float m_FixedUpdateCounter = 0.0f;

	void DrawVisualDebugging(const FVector& LastLocation, const FVector& CurrentLocation, const FVector& Gravity, const FVector& Velocity);
	
	void FindNearestPlanetToOrbit();

	UStaticMeshComponent* GetStaticMeshComponentFromOwner();

};
