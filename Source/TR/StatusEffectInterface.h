
#pragma once

#include "CoreMinimal.h"
#include "Enum/enum.h"
#include "UObject/Interface.h"
#include "StatusEffectInterface.generated.h"

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
		StatusType EffectType
	);
};
