#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EquipItemMeshActor.generated.h"

UCLASS()
class TEAM06_API AEquipItemMeshActor : public AActor
{
	GENERATED_BODY()

public:
	AEquipItemMeshActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipItem")
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipItem")
	UStaticMeshComponent* MeshComp;
};
