// Copyright (c) 2018 fivefingergames.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "OrbitalMechanics/Actors/Orbiter.h"
#include "DebrisSpawnerComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogDebrisSpawnerComponent, Log, All);

UENUM()
enum class EOrbitStartLocation : uint8
{
	Equater,
	Polar
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SPACEDEBRISJANITOR_API UDebrisSpawnerComponent : public UActorComponent
{
	GENERATED_BODY()

public:		
	UDebrisSpawnerComponent();
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner")
	TSubclassOf<class AOrbiter> DebrisToSpawn;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner")
	class APlanet* PlanetForDebris;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner")
	bool SpawnDebrisWithDebuggingEnabled = false;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner", meta = (ClampMin = "0", ClampMax = "1000", UIMin = "0", UIMax = "1000"))
	int32 DebrisCount;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SpawnHeightMinPercent;

	UPROPERTY(EditAnywhere, Category = "Debris Spawner", meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SpawnHeightMaxPercent;
	
	UFUNCTION(BlueprintCallable, Category = "Debris Spawner")
	void SpawnDebris();

protected:
	virtual void BeginPlay() override;		
	
	TArray<FVector> GenerateLocations();
	FVector GetInitialDirection(FVector InitialLocation);

};
