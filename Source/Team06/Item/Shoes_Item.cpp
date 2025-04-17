#include "Item/Shoes_Item.h"
#include "Player/Player/PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


AShoes_Item::AShoes_Item()
{
	PrimaryActorTick.bCanEverTick = false;


}

void AShoes_Item::BeginPlay()
{
	Super::BeginPlay();
}

void AShoes_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	APlayerBase* Player = Cast<APlayerBase>(OverlappedActor);
	if (!Player || !Player->GetCharacterMovement())
	{

		return;
	}

	auto Move = Player->GetCharacterMovement();
	const float OriginalSpeed = Move->MaxWalkSpeed;
	const float OriginalJump = Move->JumpZVelocity;

	// 속도 및 점프력 증가
	Move->MaxWalkSpeed += SpeedBoost;
	Move->JumpZVelocity += JumpBoost;




	if (LoopSound)
	{
		LoopSoundComp = UGameplayStatics::SpawnSoundAttached(
			LoopSound, Player->GetRootComponent(),
			NAME_None, FVector::ZeroVector,
			EAttachLocation::KeepRelativeOffset,
			true // Loop
		);
	}

	// 아이템이 사라지기 직전 → 서버에서 멀티캐스트로 시각 이펙트 제거
	if (HasAuthority())
	{
		Multicast_PlayLoopEffect(Player); // ItemBase의 이펙트 제거 함수 (서버 → 모든 클라이언트)
	}

	// 일정 시간 후 부스트 해제 및 사운드 종료
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, Player, OriginalSpeed, OriginalJump]()
		{
			if (IsValid(Player) && Player->GetCharacterMovement())
			{
				Player->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
				Player->GetCharacterMovement()->JumpZVelocity = OriginalJump;

			}

			StopLoopSound();

			if (HasAuthority())
			{
				Multicast_StopLoopEffect(); //  루프 이펙트 종료
			}

		}, Duration, false);
}
void AShoes_Item::StopLoopSound()
{
	if (LoopSoundComp && LoopSoundComp->IsPlaying())
	{
		LoopSoundComp->FadeOut(0.5f, 0.0f); // 자연스럽게 사라지게
		LoopSoundComp = nullptr;
	}
}

void AShoes_Item::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	StopLoopSound();
}