#include "Item/Sticky_Item.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

ASticky_Item::ASticky_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 캐릭터 무시!

	// SlowArea 초기화
	SlowArea = CreateDefaultSubobject<USphereComponent>(TEXT("SlowArea"));
	SlowArea->SetupAttachment(RootComponent);
	SlowArea->InitSphereRadius(100.0f);
	SlowArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 꺼두기!
	SlowArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	SlowArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 캐릭터만 감지
}

void ASticky_Item::BeginPlay()
{
	Super::BeginPlay();

	if (SlowArea)
	{
		SlowArea->OnComponentBeginOverlap.AddDynamic(this, &ASticky_Item::OnOverlapBegin);
	}

	if (bDebugDrawArea)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SlowArea->GetScaledSphereRadius(), 16, FColor::Cyan, true, 9999.f, 0, 1.5f);
	}
}

void ASticky_Item::ServerUseItem_Implementation(AActor* Target)
{
	if (!bIsActivated)
	{
		bIsActivated = true;

		UE_LOG(LogTemp, Warning, TEXT("[StickyItem] 아이템 사용됨 → 착지 대기 상태"));
	}
}

void ASticky_Item::OnItemLanded_Implementation()
{
	Super::OnItemLanded_Implementation();

	if (!bIsActivated || bIsDestroyed) return;

	UE_LOG(LogTemp, Warning, TEXT("[StickyItem] 착지 감지됨 → 트랩 발동 시작"));
	// 착지 시 슬로우 발동
	ActivateStickyTrap();
}

void ASticky_Item::ActivateStickyTrap()
{
	if (bIsDestroyed) return;
	UE_LOG(LogTemp, Warning, TEXT("[StickyItem] 슬로우 트랩 발동!"));

	// 슬로우 범위 디버그 시각화 (5초 유지)
	if (bDebugDrawArea)
	{
		DrawDebugSphere(GetWorld(), GetActorLocation(), SlowArea->GetScaledSphereRadius(), 16, FColor::Green, false, 5.0f);
	}

	// 메시 비활성화 (던진 아이템 숨기기)
	if (ItemMesh)
	{
		// 캐릭터 충돌 다시 활성화 (슬로우 걸기용)
		ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

		SlowArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	// 제거 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ASticky_Item::SafeDestroy, DestroyDelay, false);
}

void ASticky_Item::SafeDestroy()
{
	if (!bIsDestroyed)
	{
		bIsDestroyed = true;
		UE_LOG(LogTemp, Warning, TEXT("[StickyItem] 트랩 종료 → 아이템 제거"));
		Destroy();
	}
}

void ASticky_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character && Character->GetCharacterMovement())
	{
		UE_LOG(LogTemp, Warning, TEXT("[StickyItem] %s 슬로우 적용!"), *Character->GetName());
		float OriginalSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
		Character->GetCharacterMovement()->MaxWalkSpeed *= SlowMultiplier;

		FTimerHandle TimerHandle;
		FTimerDelegate TimerDel;
		TimerDel.BindLambda([Character, OriginalSpeed]() {
			if (Character && Character->GetCharacterMovement())
			{
				Character->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
				UE_LOG(LogTemp, Warning, TEXT("[StickyItem] %s 속도 복원"), *Character->GetName());
			}
			});
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDel, SlowDuration, false);
	}
}