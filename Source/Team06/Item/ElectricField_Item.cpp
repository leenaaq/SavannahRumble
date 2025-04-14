#include "Item/ElectricField_Item.h"
#include "NiagaraFunctionLibrary.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AElectricField_Item::AElectricField_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	ItemID = "BP_ElectricField_Item";

	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemMesh->SetCollisionResponseToAllChannels(ECR_Block);
	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore); // 캐릭터 충돌 무시

	FieldArea = CreateDefaultSubobject<USphereComponent>(TEXT("FieldArea"));
	FieldArea->SetupAttachment(RootComponent);
	FieldArea->InitSphereRadius(250.f);
	FieldArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 비활성화
	FieldArea->SetCollisionResponseToAllChannels(ECR_Ignore);
	FieldArea->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); // 캐릭터만 감지
}

void AElectricField_Item::BeginPlay()
{
	Super::BeginPlay();

	// 충돌 감지 바인딩
	if (FieldArea)
	{
		FieldArea->OnComponentBeginOverlap.AddDynamic(this, &AElectricField_Item::OnOverlapBegin);
	}
	OnItemLanded();
}

void AElectricField_Item::ServerUseItem_Implementation(AActor* Target)
{
	// 사용되었다고 표시
	if (!bIsActivated)
	{
		bIsActivated = true;
		UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 아이템 사용됨 → 착지 대기 상태"));
	}
}

void AElectricField_Item::OnItemLanded_Implementation()
{
	Super::OnItemLanded_Implementation();

	if (!bIsActivated || bHasLanded) return;

	bHasLanded = true;

	ItemMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	FieldArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 착지 감지됨 → 자기장 발동 시작"));

	ActivateElectricField();
}

void AElectricField_Item::ActivateElectricField()
{
	// 이펙트 실행
	if (FieldEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), FieldEffect, GetActorLocation(), GetActorRotation());
		UE_LOG(LogTemp, Log, TEXT("[ElectricField] 나이아가라 이펙트 실행됨"));
	}

	// 메시 감추고 충돌 제거 (던진 아이템 비활성화)
	if (ItemMesh)
	{
		ItemMesh->SetVisibility(false, true);
		ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	// 자기장 영역 활성화
	FieldArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 디버그 구체
	DrawDebugSphere(GetWorld(), GetActorLocation(), FieldArea->GetScaledSphereRadius(), 16, FColor::Yellow, false, FieldDuration);

	// 일정 시간 후 제거
	GetWorld()->GetTimerManager().SetTimer(ActivationTimerHandle, this, &AElectricField_Item::DestroySelf, FieldDuration, false);

	UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 자기장 활성화 완료 → %f초 후 제거 예정"), FieldDuration);
}

void AElectricField_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerBase* Player = Cast<APlayerBase>(OtherActor);
	if (Player && !Player->GetbIsStunned() && !AffectedPlayers.Contains(Player))
	{
		AffectedPlayers.Add(Player);
		Player->OnStunned(FieldDuration);

		UE_LOG(LogTemp, Warning, TEXT("[ElectricField] %s → OnStunned(%f) 호출됨"), *Player->GetName(), FieldDuration);
	}
}

void AElectricField_Item::DestroySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 자기장 제거"));
	Destroy();
}