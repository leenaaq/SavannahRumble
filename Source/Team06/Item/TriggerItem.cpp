#include "Item/TriggerItem.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

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
	if (HasAuthority())
	{
		// 서버에서 처리
		ServerTriggerEffect(OverlappedActor);
	}
	else
	{
		// 클라에서는 서버에 요청만
		ServerTriggerEffect(OverlappedActor);
	}
	// 기본 동작: 로그만 출력
	UE_LOG(LogTemp, Log, TEXT("[TriggerItem] TriggerEffect 호출됨 - 대상: %s"), *OverlappedActor->GetName());
}

void ATriggerItem::ServerTriggerEffect_Implementation(AActor* OverlappedActor)
{
	// 실제 효과 처리 로직
	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character)
	{
		// 점프 날리기 등 서버 전용 로직
	}
}