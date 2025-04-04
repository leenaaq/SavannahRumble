#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"


class UBoxComponent;
class USceneComponent;
class AItem;
class AItemSpawnPoint;

UCLASS()
class TEAM06_API AItemSpawner : public AActor
{
	GENERATED_BODY()

public:
	AItemSpawner();

protected:
	virtual void BeginPlay() override;

private:

	FTimerHandle DebugLogHandle;

	// 아이템을 생성할 스폰 포인트 목록
	UPROPERTY()
	TArray<AItemSpawnPoint*> SpawnPoints;

	// 스폰할 수 있는 아이템 클래스 목록
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	TArray<TSubclassOf<class AItem>> ItemClasses;

	// 아이템 리스폰 시간 (초)
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	float RespawnTime = 3.0f;

	// 각 스폰 포인트에 대한 현재 스폰된 아이템 관리
	TMap<AItemSpawnPoint*, AItem*> SpawnedItems;

	// 박스 범위 (스폰 영역 시각화용)
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<UBoxComponent> SpawnArea;

	// 스폰할 위치의 Scene 루트
	UPROPERTY(VisibleAnywhere, Category = "Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UFUNCTION()
	void DebugAutoRemoveAfterDelay();

	// 아이템 스폰 함수
	void SpawnItems();

	// 아이템 제거 및 리스폰 함수
	void RespawnItem(AItemSpawnPoint* SpawnPoint);

	// 디버그용 함수들 추후 삭제
	void LogSpawnerStatus();
	// 디버그용 함수들 추후 삭제
	void DebugRemoveAllItems();
	// 디버그용 자동 아이템 제거 함수
	
	// 특정 아이템이 제거될 때 호출
	UFUNCTION()
	void OnItemDestroyed(AActor* DestroyedActor);
};