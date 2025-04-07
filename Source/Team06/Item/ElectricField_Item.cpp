#include "Item/ElectricField_Item.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

AElectricField_Item::AElectricField_Item()
{
	PrimaryActorTick.bCanEverTick = false;

	// 기본 메쉬
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	// 전기장 이펙트 (기본 비활성화)
	ElectricEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ElectricEffect"));
	ElectricEffect->SetupAttachment(RootComponent);
	ElectricEffect->SetAutoActivate(false);

	// 감지 영역 (기본 비활성화)
	FieldArea = CreateDefaultSubobject<USphereComponent>(TEXT("FieldArea"));
	FieldArea->SetupAttachment(RootComponent);
	FieldArea->InitSphereRadius(250.f);
	FieldArea->SetCollisionEnabled(ECollisionEnabled::NoCollision); // 처음엔 꺼둠
}

void AElectricField_Item::BeginPlay()
{
	Super::BeginPlay();

	FieldArea->OnComponentBeginOverlap.AddDynamic(this, &AElectricField_Item::OnOverlapBegin);

	// 착지 후 전기장 활성화 대기
	GetWorld()->GetTimerManager().SetTimer(ActivationTimer, this, &AElectricField_Item::ActivateElectricField, ActivationDelay, false);
}

void AElectricField_Item::ActivateElectricField()
{
	// 멀티캐스트로 모든 클라이언트에 전기장 발동 알림
	MulticastActivateField();

	// 필드 제거 타이머
	GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &AElectricField_Item::DestroySelf, FieldDuration, false);
}

void AElectricField_Item::MulticastActivateField_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 전기장 발동"));

	// 메쉬 숨기기, 이펙트 켜기
	BaseMesh->SetVisibility(false);
	ElectricEffect->SetActive(true);

	// 충돌 감지 시작
	FieldArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	FieldArea->SetCollisionResponseToAllChannels(ECR_Overlap);
}


void AElectricField_Item::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacter* Character = Cast<ACharacter>(OtherActor);
	if (Character)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ElectricField] %s 전기장에 닿음 → Ragdoll"), *Character->GetName());

		Character->GetMesh()->SetSimulatePhysics(true);
		Character->GetCharacterMovement()->DisableMovement();

		// 일정 시간 뒤 복구
		FTimerHandle RagdollTimer;
		FTimerDelegate RagdollRestore;
		RagdollRestore.BindLambda([Character]()
			{
				if (Character)
				{
					Character->GetMesh()->SetSimulatePhysics(false);
					Character->GetCharacterMovement()->SetMovementMode(MOVE_Walking);
					UE_LOG(LogTemp, Warning, TEXT("[ElectricField] %s Ragdoll 해제"), *Character->GetName());
				}
			});
		GetWorld()->GetTimerManager().SetTimer(RagdollTimer, RagdollRestore, RagdollTime, false);
	}
}

void AElectricField_Item::DestroySelf()
{
	UE_LOG(LogTemp, Warning, TEXT("[ElectricField] 자기장 제거"));
	Destroy();
}