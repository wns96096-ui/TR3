
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TR/Enum/enum.h"
#include "trap_BP_assets.generated.h"

class Atrap_base;

/**
 * 
 */


UCLASS(BlueprintType)
class TR_API Utrap_BP_assets : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Object")
	TMap<TrapType, TSubclassOf<Atrap_base>> ObjClass;

	UFUNCTION(BlueprintPure, Category="Object")
	TSubclassOf<Atrap_base> GetTrapClass(TrapType TrapType) const;
};
