#include "Item/Shoes_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

void AShoes_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character && Character->GetCharacterMovement())
	{
		UCharacterMovementComponent* Movement = Character->GetCharacterMovement();

		// 기존 값 저장
		const float OriginalSpeed = Movement->MaxWalkSpeed;
		const float OriginalJumpZ = Movement->JumpZVelocity;

		// 부스트 적용
		Movement->MaxWalkSpeed += SpeedBoost;
		Movement->JumpZVelocity += JumpBoost;

		UE_LOG(LogTemp, Log, TEXT("[Shoes_Item] %s 캐릭터에 부스트 적용됨 (속도 +%.1f, 점프력 +%.1f)"),
			*Character->GetName(), SpeedBoost, JumpBoost);

		// 타이머로 원상 복귀
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, [this, Character, OriginalSpeed, OriginalJumpZ]()
			{
				if (IsValid(Character) && Character->GetCharacterMovement())
				{
					UCharacterMovementComponent* MovementComp = Character->GetCharacterMovement();
					MovementComp->MaxWalkSpeed = OriginalSpeed;
					MovementComp->JumpZVelocity = OriginalJumpZ;

					UE_LOG(LogTemp, Log, TEXT("[Shoes_Item] %s 캐릭터 부스트 효과 종료 (속도, 점프력 원상 복귀)"), *Character->GetName());
				}
			}, Duration, false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Shoes_Item] TriggerEffect 실패 - 유효하지 않은 캐릭터입니다."));
	}
}
