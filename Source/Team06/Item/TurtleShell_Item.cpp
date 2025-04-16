#include "Item/TurtleShell_Item.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

void ATurtleShell_Item::TriggerEffect_Implementation(AActor* OverlappedActor)
{
	Super::TriggerEffect_Implementation(OverlappedActor);

	ACharacter* Character = Cast<ACharacter>(OverlappedActor);
	if (Character && Character->GetCharacterMovement())
	{
		// 밟았을 때 사운드 재생
		if (BounceSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, GetActorLocation());
		}

		// 캐릭터 튕기기
		Character->LaunchCharacter(FVector(0, 0, BounceImpulse), false, true);

		
	}
	else
	{
		
	}
}
