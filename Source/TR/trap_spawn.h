#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Enum/enum.h"
#include "trap_spawn.generated.h"

class Atrap_base;
class Utrap_BP_assets;

UCLASS()
class TR_API Utrap_spawn : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// UFUNCTION(BlueprintCallable, Category="Trap")
	// static TArray<Atrap_base*> SpawnTest1(
	// 	AActor* OriginActor,
	// 	Utrap_BP_assets* TrapAssets,
	// 	TrapType TrapType,
	// 	float YOffset = 4000.0f,
	// 	float XStart = 0.0f,
	// 	float XEnd = 1200.0f,
	// 	int32 SpawnCount = 6
	// );
	//
	// // UFUNCTION(BlueprintCallable, Category="Trap")
	// static TArray<Atrap_base*> SpawnTest2(
	// 	AActor* OriginActor,
	// 	Utrap_BP_assets* TrapAssets,
	// 	TrapType TrapType,
	// 	const TArray<Atrap_base*>& ExistingTraps,
	// 	float ExclusionDistance = 2000.0f,
	// 	float YOffset = 4000.0f,
	// 	float XStart = 0.0f,
	// 	float XEnd = 1200.0f,
	// 	int32 SpawnCount = 6
	// );

	UFUNCTION(BlueprintCallable, Category="Trap")
	static TArray<Atrap_base*> SpawnTrap(
		AActor* OriginActor,
		Utrap_BP_assets* TrapAssets,
		TrapType TrapType,
		const TArray<Atrap_base*>& ExistingTraps,
		float ExclusionDistance = 2000.0f,
		float YOffset = 4000.0f,
		float XStart = 0.0f,
		float XEnd = -1200.0f,
		int32 SlotCount = 6,
		int32 SpawnCount = 3
	);

	UFUNCTION(BlueprintCallable, Category="Trap")
	static void DestroySpawnedTraps(UPARAM(ref) TArray<Atrap_base*>& Traps);
};
