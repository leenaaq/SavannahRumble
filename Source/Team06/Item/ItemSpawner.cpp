#include "ItemSpawner.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DataTable.h"


AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(SceneRoot);
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpawnArea->SetGenerateOverlapEvents(false);
	SpawnArea->bHiddenInGame = true;
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AItemSpawnPoint::StaticClass(), FoundActors);

		for (AActor* Actor : FoundActors)
		{
			if (AItemSpawnPoint* SpawnPoint = Cast<AItemSpawnPoint>(Actor))
			{
				SpawnPoints.Add(SpawnPoint);
			}
		}

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
		TotalChance += Row->SpawnChance;
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
			AActor* SpawnedItem = GetWorld()->SpawnActor<AActor>(
				SelectedRow->ItemClass,
				SpawnPoint->GetActorLocation(),
				FRotator::ZeroRotator
			);

			if (SpawnedItem)
			{
				UE_LOG(LogTemp, Log, TEXT("[ItemSpawner] 아이템 생성 완료: %s (스폰 위치: %s)"),
					*SpawnedItem->GetName(), *SpawnPoint->GetName());

				if (SpawnedItem->GetClass()->ImplementsInterface(URespawnableInterface::StaticClass()))
				{
					SpawnedItem->OnDestroyed.AddDynamic(this, &AItemSpawner::OnItemDestroyed);
				}

				SpawnedItems.Add(SpawnPoint, SpawnedItem);
			}
		}
	}
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	if (!DestroyedActor) return;

	for (auto& Elem : SpawnedItems)
	{
		if (Elem.Value == DestroyedActor)
		{
			AItemSpawnPoint* SpawnPoint = Elem.Key;
			SpawnedItems[SpawnPoint] = nullptr;

			// 리스폰 인터페이스 구현 확인
			if (DestroyedActor->GetClass()->ImplementsInterface(URespawnableInterface::StaticClass()))
			{
				IRespawnableInterface::Execute_OnRespawned(DestroyedActor); // 선택적으로 사용

				FTimerHandle RespawnTimerHandle;
				GetWorld()->GetTimerManager().SetTimer(RespawnTimerHandle, FTimerDelegate::CreateLambda([this, SpawnPoint]()
					{
						if (SpawnPoint)
						{
							this->SpawnItems();
						}
					}), RespawnTime, false);
			}

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

