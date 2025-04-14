#include "Item/Banana_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

void ABanana_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character && Character->GetMesh())
	{
		//효과음 재생 (모든 클라이언트에서 들림)
		if (SlipSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), SlipSound, GetActorLocation());
		}

		// 로그
		UE_LOG(LogTemp, Log, TEXT("[Banana_Item] %s 캐릭터가 바나나에 의해 랙돌 상태로 전환됨. 슬라이드 힘: %f"), *Character->GetName(), SlideForce);

		// 랙돌
		Character->GetMesh()->SetSimulatePhysics(true);

		// 앞으로 밀기
		FVector Forward = Character->GetActorForwardVector();
		Character->GetMesh()->AddImpulse(Forward * SlideForce, NAME_None, true);

		// 일정 시간 뒤 원상 복귀
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [Character]()
			{
				if (IsValid(Character) && Character->GetMesh())
				{
					Character->GetMesh()->SetSimulatePhysics(false);
					UE_LOG(LogTemp, Log, TEXT("[Banana_Item] %s 캐릭터 랙돌 해제됨"), *Character->GetName());
				}
			}, RagdollDuration, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Banana_Item] 랙돌 적용 실패 - 유효하지 않은 캐릭터 또는 메시."));
	}
}