
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StatusEffectInterface.generated.h"
UENUM(BlueprintType)
enum class EStatusEffectType : uint8
{
	Stun,
	Slow,
	Down,
	Electric,
	
};

UINTERFACE(BlueprintType)
class TR_API UStatusEffectReceiver : public  UInterface
{
	GENERATED_BODY()
};

class TR_API IStatusEffectReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Effect Type")
	void ApplyStatusEffect(
		EStatusEffectType EffectType
	);
};