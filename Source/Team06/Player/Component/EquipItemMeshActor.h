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

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* MeshComponent;

	UFUNCTION(BlueprintCallable)
	void UpdateMesh(UStaticMesh* NewMesh);
};
