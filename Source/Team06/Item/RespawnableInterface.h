#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RespawnableInterface.generated.h"

UINTERFACE(MinimalAPI)
class URespawnableInterface : public UInterface
{
	GENERATED_BODY()
};

class TEAM06_API IRespawnableInterface
{
	GENERATED_BODY()

public:
   
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Respawn")
    void OnRespawned();
};
