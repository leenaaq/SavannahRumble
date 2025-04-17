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
			if (AItemSpawnPoint* Point = Cast<AItemSpawnPoint>(Actor))
			{
				SpawnPoints.Add(Point);
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
		
	for (AItemSpawnPoint* SpawnPoint : SpawnPoints)
		
	{	
		if (SpawnedItems.Contains(SpawnPoint) && SpawnedItems[SpawnPoint]) continue;	

		FItemSpawnRow* Row = GetRandomItem();
		if (!Row || !Row->ItemClass) continue;
		AActor* Spawned = GetWorld()->SpawnActor<AActor>(
				Row->ItemClass,
				SpawnPoint->GetActorLocation(),
				FRotator::ZeroRotator
		);
		if (IsValid(Spawned) && Spawned->GetClass()->ImplementsInterface(URespawnableInterface::StaticClass()))
		{
			
			Spawned->OnDestroyed.AddDynamic(this, &AItemSpawner::OnItemDestroyed);
			SpawnedItems.Add(SpawnPoint, Spawned);
		}
	}
}

void AItemSpawner::OnItemDestroyed(AActor* DestroyedActor)
{
	for (auto& Elem : SpawnedItems)
	{
		if (Elem.Value == DestroyedActor)
		{
			AItemSpawnPoint* Point = Elem.Key;
			SpawnedItems[Point] = nullptr;

		
			if (DestroyedActor->GetClass()->ImplementsInterface(URespawnableInterface::StaticClass()))
			{
				IRespawnableInterface::Execute_OnRespawned(DestroyedActor);

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Point]()
					{
						if (Point) SpawnItems();
					}, RespawnTime, false);
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

