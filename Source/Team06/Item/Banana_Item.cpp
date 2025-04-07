#include "Item/Banana_Item.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void ABanana_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character && Character->GetMesh())
	{
		// 디버그 로그
		UE_LOG(LogTemp, Log, TEXT("[Banana_Item] %s 캐릭터가 바나나에 의해 랙돌 상태로 전환됨. 슬라이드 힘: %f"), *Character->GetName(), SlideForce);

		// 랙돌 활성화
		Character->GetMesh()->SetSimulatePhysics(true);

		// 앞으로 밀기
		FVector Forward = Character->GetActorForwardVector();
		Character->GetMesh()->AddImpulse(Forward * SlideForce, NAME_None, true);

		// 일정 시간 후 랙돌 해제
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [Character, this]()
			{
				if (IsValid(Character) && Character->GetMesh())
				{
					Character->GetMesh()->SetSimulatePhysics(false);
					UE_LOG(LogTemp, Log, TEXT("[Banana_Item] %s 캐릭터가 원래 상태로 복귀됨 (랙돌 해제)"), *Character->GetName());
				}
			}, RagdollDuration, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Banana_Item] 랙돌 적용 실패 - 유효하지 않은 캐릭터 또는 메시."));
	}
}