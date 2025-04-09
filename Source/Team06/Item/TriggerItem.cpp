#include "Item/TriggerItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

ATriggerItem::ATriggerItem()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMesh"));
	RootComponent = ItemMesh;

	TriggerArea = CreateDefaultSubobject<USphereComponent>(TEXT("TriggerArea"));
	TriggerArea->SetupAttachment(ItemMesh);
	TriggerArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
}

void ATriggerItem::BeginPlay()
{
	Super::BeginPlay();
	
	// 충돌 이벤트 연결
	if (TriggerArea)
	{
		TriggerArea->OnComponentBeginOverlap.AddDynamic(this, &ATriggerItem::OnOverlapBegin);
	}
}

void ATriggerItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (!DebugMessage.IsEmpty())
		{
			UE_LOG(LogTemp, Warning, TEXT("[TriggerItem] %s"), *DebugMessage);
		}

		// 트리거 효과 실행
		TriggerEffect(OtherActor);

		// 자동 제거 옵션이 켜져 있다면 아이템 제거
		if (bAutoDestroyOnTrigger)
		{
			Destroy();
		}
	}
}

void ATriggerItem::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	// 서버에서만 실행
	if (!HasAuthority()) return;

	// 이펙트 & 사운드 재생
	PlayItemEffects(GetActorLocation());

	// 추가 로직이 필요한 경우 자식에서 오버라이드
	UE_LOG(LogTemp, Log, TEXT("[TriggerItem] TriggerEffect 실행됨 - 대상: %s"), *OverlappedActor->GetName());
}
