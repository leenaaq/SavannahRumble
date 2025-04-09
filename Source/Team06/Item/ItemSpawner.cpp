#include "ItemSpawner.h"
#include "ItemSpawnPoint.h"
#include "Item/EquipableItem.h"
#include "Item/TriggerItem.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"


AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(SceneRoot);

	// 게임에는 영향 주지 않고 에디터에서만 확인 가능하게 설정
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetGenerateOverlapEvents(false);
	SpawnArea->bHiddenInGame = true;
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		// 레벨에서 모든 `AItemSpawnPoint` 찾기
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnPoint::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (AItemSpawnPoint* SpawnPoint = Cast<AItemSpawnPoint>(Actor))
			{
				SpawnPoints.Add(SpawnPoint);
			}
		}

		// 아이템 초기 스폰
		SpawnItems();

	}
}

FItemSpawnRow* AItemSpawner::GetRandomItem() const
{
	if (!ItemDataTable) return nullptr;

	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));
	ItemDataTable->GetAllRows<FItemSpawnRow>(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if (Row) TotalChance += Row->SpawnChance;
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulateChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		AccumulateChance += Row->SpawnChance;
		if (RandValue <= AccumulateChance)
		{
			return Row;
		}
	}

	return nullptr;
}

void AItemSpawner::SpawnItems()
{
	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ItemSpawner] 스폰 포인트가 비어 있습니다."));
		return;
	}

	for (AItemSpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (SpawnedItems.Contains(SpawnPoint) && SpawnedItems[SpawnPoint] != nullptr)
		{
			continue;
		}

		FItemSpawnRow* SelectedRow = GetRandomItem();
		if (SelectedRow && SelectedRow->ItemClass)
		{
			AEquipableItem* SpawnedItem = GetWorld()->SpawnActor<AEquipableItem>(
				SelectedRow->ItemClass,
				SpawnPoint->GetActorLocation(),
				FRotator::ZeroRotator
			);

			if (SpawnedItem)
			{
				UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] 아이템 생성 완료: %s (스폰 위치: %s)"), *SpawnedItem->GetName(), *SpawnPoint->GetName());
				SpawnedItem->OnDestroyed.AddDynamic(this, &AItemSpawner::OnItemDestroyed);
				SpawnedItems.Add(SpawnPoint, SpawnedItem);
			}
		}
	}
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor) return;

	AEquipableItem* DestroyedItem = Cast<AEquipableItem>(DestroyedActor);
	if (!DestroyedItem) return;

	// 아이템이 실제로 삭제되었는지 확인
	UE_LOG(LogTemp, Warning, TEXT("[ItemSpawner] 아이템 삭제됨: %s"), *DestroyedItem->GetName());

	// 삭제된 아이템의 스폰 포인트 찾기
	for (auto& Elem : SpawnedItems)
	{
		if (Elem.Value == DestroyedItem)
		{
			AItemSpawnPoint* SpawnPoint = Elem.Key;
			SpawnedItems[SpawnPoint] = nullptr; // 아이템 삭제됨 표시

			// 아이템이 삭제됐을 때 리스폰 타이머가 정상 작동하는지 확인
			UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] %.1f초 후 [%s]에서 아이템 리스폰 예정"), RespawnTime, *SpawnPoint->GetName());

			// 일정 시간 후 아이템 재생성
			FTimerHandle RespawnTimerHandle;
			GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda([this, SpawnPoint]()
				{
					if (SpawnPoint)
					{
						// 삭제 후 아이템이 정상적으로 생성되는지 확인
						UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] 리스폰 타이머 발동 - 아이템 재스폰 시도"));
						this->SpawnItems();
					}
				}), RespawnTime, false);

			break;
		}
	}
}

void AItemSpawner::RespawnItem(AItemSpawnPoint* SpawnPoint)
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		FTimerDelegate::CreateUObject(this, &AItemSpawner::SpawnItems),
		RespawnTime,
		false
	);
}

