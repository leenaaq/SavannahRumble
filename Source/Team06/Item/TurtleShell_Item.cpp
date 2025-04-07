#include "Item/TurtleShell_Item.h"
#include "GameFramework/Character.h"

void ATurtleShell_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character && Character->GetCharacterMovement())
	{
		Character->LaunchCharacter(FVector(0, 0, BounceImpulse), false, true);

		// 디버그 로그 출력
		UE_LOG(LogTemp, Log, TEXT("[TurtleShell_Item] %s 캐릭터가 거북이 등껍질에 의해 %f의 힘으로 튕겨졌습니다."), *Character->GetName(), BounceImpulse);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[TurtleShell_Item] 튕기기 실패 - 캐릭터 또는 MovementComponent가 없음."));
	}
}
