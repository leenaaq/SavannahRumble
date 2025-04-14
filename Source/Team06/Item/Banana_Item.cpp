#include "Item/Banana_Item.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ABanana_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	APlayerBase* Player = Cast<APlayerBase>(OverlappedActor);
	if (!Player || !Player->GetMesh()) return;

	// 사운드
	if (SlipSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlipSound, GetActorLocation());
	}

	// 로그
	UE_LOG(LogTemp, Warning, TEXT("[Banana_Item] %s slipped on banana!"), *Player->GetName());

	// 기절 처리
	Player->OnStunned(Player->GetStats().StunDuration); // or 직접 값 지정
	Player->SetbIsStunned(true);

	// 물리
	USkeletalMeshComponent* Mesh = Player->GetMesh();
	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(FName("Ragdoll"));

	// Impulse 적용
	const FVector Impulse = Player->GetActorForwardVector() * SlideForce;
	Mesh->AddImpulse(Impulse, NAME_None, true);
	UE_LOG(LogTemp, Warning, TEXT("[Banana_Item] Impulse applied: %s"), *Impulse.ToString());

	// 일정 시간 후 복구
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Player]()
		{
			if (Player && Player->GetMesh())
			{
				Player->GetMesh()->SetSimulatePhysics(false);
				Player->GetMesh()->SetCollisionProfileName(FName("CharacterMesh"));

				Player->SetbIsStunned(false);
				Player->MulticastRecoverFromStun_Implementation(); // 상태 복구 (모든 클라)

				UE_LOG(LogTemp, Warning, TEXT("[Banana_Item] %s 랙돌 복구됨"), *Player->GetName());
			}
		}, RagdollDuration, false);
}