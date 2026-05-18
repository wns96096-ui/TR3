#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TrapDestroyInterface.generated.h"

class AActor;

UINTERFACE(BlueprintType)
class TR_API UTrapDestroyReceiver : public UInterface
{
	GENERATED_BODY()
};

class TR_API ITrapDestroyReceiver
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Trap")
	void OnTrapDestroyTriggered(AActor* TrapActor);
};
