#include "trap_spawn.h"

#include "DA/trap_BP_assets.h"
#include "Engine/World.h"
#include "trap_base.h"

namespace
{
bool IsNearExistingTrapStartLocation(
	const FVector& SpawnLocation,
	const TArray<Atrap_base*>& ExistingTraps,
	float ExclusionDistance
)
{
	const float ExclusionDistanceSquared = FMath::Square(ExclusionDistance);

	for (const Atrap_base* ExistingTrap : ExistingTraps)
	{
		if (!IsValid(ExistingTrap))
		{
			continue;
		}

		if (FVector::DistSquared2D(SpawnLocation, ExistingTrap->GetInitialSpawnLocation()) <= ExclusionDistanceSquared)
		{
			return true;
		}
	}

	return false;
}
}
//
// TArray<Atrap_base*> Utrap_spawn::SpawnTest1(
// 	AActor* OriginActor,
// 	Utrap_BP_assets* TrapAssets,
// 	TrapType TrapType,
// 	float YOffset,
// 	float XStart,
// 	float XEnd,
// 	int32 SpawnCount
// )
// {
// 	TArray<Atrap_base*> SpawnedTraps;
//
// 	if (!OriginActor || !TrapAssets || SpawnCount <= 0)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	UWorld* World = OriginActor->GetWorld();
// 	if (!World)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	TSubclassOf<Atrap_base> TrapClass = TrapAssets->GetTrapClass(TrapType);
// 	if (!TrapClass)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	SpawnedTraps.Reserve(SpawnCount);
//
// 	const FVector OriginLocation = OriginActor->GetActorLocation();
// 	const FRotator SpawnRotation = OriginActor->GetActorRotation();
// 	const float XStep = SpawnCount > 1 ? (XEnd - XStart) / static_cast<float>(SpawnCount - 1) : 0.0f;
//
// 	FActorSpawnParameters SpawnParams;
// 	SpawnParams.Owner = OriginActor;
// 	SpawnParams.Instigator = OriginActor->GetInstigator();
// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
// 	for (int32 Index = 0; Index < SpawnCount; ++Index)
// 	{
// 		const float XOffset = XStart + (XStep * Index);
// 		const FVector SpawnLocation = FVector(
// 		XOffset,
// 		OriginLocation.Y + YOffset,
// 		0.0f
// 	);
// 		Atrap_base* SpawnedTrap = World->SpawnActor<Atrap_base>(
// 			TrapClass,
// 			SpawnLocation,
// 			SpawnRotation,
// 			SpawnParams
// 		);
//
// 		if (SpawnedTrap)
// 		{
// 			SpawnedTraps.Add(SpawnedTrap);
// 		}
// 	}
//
// 	return SpawnedTraps;
// }
//
// TArray<Atrap_base*> Utrap_spawn::SpawnTest2(
// 	AActor* OriginActor,
// 	Utrap_BP_assets* TrapAssets,
// 	TrapType TrapType,
// 	const TArray<Atrap_base*>& ExistingTraps,
// 	float ExclusionDistance,
// 	float YOffset,
// 	float XStart,
// 	float XEnd,
// 	int32 SpawnCount
// )
// {
// 	TArray<Atrap_base*> SpawnedTraps;
//
// 	if (!OriginActor || !TrapAssets || SpawnCount <= 0)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	UWorld* World = OriginActor->GetWorld();
// 	if (!World)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	TSubclassOf<Atrap_base> TrapClass = TrapAssets->GetTrapClass(TrapType);
// 	if (!TrapClass)
// 	{
// 		return SpawnedTraps;
// 	}
//
// 	SpawnedTraps.Reserve(SpawnCount);
//
// 	const FVector OriginLocation = OriginActor->GetActorLocation();
// 	const FRotator SpawnRotation = OriginActor->GetActorRotation();
// 	const float XStep = SpawnCount > 1 ? (XEnd - XStart) / static_cast<float>(SpawnCount - 1) : 0.0f;
//
// 	FActorSpawnParameters SpawnParams;
// 	SpawnParams.Owner = OriginActor;
// 	SpawnParams.Instigator = OriginActor->GetInstigator();
// 	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
//
// 	for (int32 Index = 0; Index < SpawnCount; ++Index)
// 	{
// 		const float XOffset = XStart + (XStep * Index);
// 		const FVector SpawnLocation = FVector(
// 			XOffset,
// 			OriginLocation.Y + YOffset,
// 			0.0f
// 		);
//
// 		if (ExclusionDistance > 0.0f && IsNearExistingTrapStartLocation(SpawnLocation, ExistingTraps, ExclusionDistance))
// 		{
// 			continue;
// 		}
//
// 		Atrap_base* SpawnedTrap = World->SpawnActor<Atrap_base>(
// 			TrapClass,
// 			SpawnLocation,
// 			SpawnRotation,
// 			SpawnParams
// 		);
//
// 		if (SpawnedTrap)
// 		{
// 			SpawnedTraps.Add(SpawnedTrap);
// 		}
// 	}
//
// 	return SpawnedTraps;
// }

TArray<Atrap_base*> Utrap_spawn::SpawnTrap(
	AActor* OriginActor,
	Utrap_BP_assets* TrapAssets,
	TrapType TrapType,
	const TArray<Atrap_base*>& ExistingTraps,
	float ExclusionDistance,
	float YOffset,
	float XStart,
	float XEnd,
	int32 SlotCount,
	int32 SpawnCount
)
{
	TArray<Atrap_base*> SpawnedTraps;

	if (!OriginActor || !TrapAssets || SlotCount <= 0 || SpawnCount <= 0)
	{
		return SpawnedTraps;
	}

	UWorld* World = OriginActor->GetWorld();
	if (!World)
	{
		return SpawnedTraps;
	}

	TSubclassOf<Atrap_base> TrapClass = TrapAssets->GetTrapClass(TrapType);
	if (!TrapClass)
	{
		return SpawnedTraps;
	}

	const int32 TargetSpawnCount = FMath::Min(SpawnCount, SlotCount);
	SpawnedTraps.Reserve(TargetSpawnCount);

	TArray<int32> SlotIndices;
	SlotIndices.Reserve(SlotCount);
	for (int32 SlotIndex = 0; SlotIndex < SlotCount; ++SlotIndex)
	{
		SlotIndices.Add(SlotIndex);
	}

	for (int32 Index = SlotIndices.Num() - 1; Index > 0; --Index)
	{
		const int32 SwapIndex = FMath::RandRange(0, Index);
		SlotIndices.Swap(Index, SwapIndex);
	}

	const FVector OriginLocation = OriginActor->GetActorLocation();
	const FRotator SpawnRotation = OriginActor->GetActorRotation();
	const float XStep = SlotCount > 1 ? (XEnd - XStart) / static_cast<float>(SlotCount - 1) : 0.0f;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OriginActor;
	SpawnParams.Instigator = OriginActor->GetInstigator();
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (const int32 SlotIndex : SlotIndices)
	{
		if (SpawnedTraps.Num() >= TargetSpawnCount)
		{
			break;
		}

		const float XOffset = XStart + (XStep * SlotIndex);
		const FVector SpawnLocation = FVector(
			XOffset,
			OriginLocation.Y + YOffset,
			0.0f
		);

		if (ExclusionDistance > 0.0f && IsNearExistingTrapStartLocation(SpawnLocation, ExistingTraps, ExclusionDistance))
		{
			continue;
		}

		Atrap_base* SpawnedTrap = World->SpawnActor<Atrap_base>(
			TrapClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (SpawnedTrap)
		{
			SpawnedTraps.Add(SpawnedTrap);
		}
	}

	return SpawnedTraps;
}

void Utrap_spawn::DestroySpawnedTraps(TArray<Atrap_base*>& Traps)
{
	for (Atrap_base* Trap : Traps)
	{
		if (IsValid(Trap))
		{
			Trap->Destroy();
		}
	}

	Traps.Empty();
}
