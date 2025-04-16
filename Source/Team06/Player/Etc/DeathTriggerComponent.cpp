#include "Player/Etc/DeathTriggerComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Player/Player/PlayerBase.h"
#include "Net/UnrealNetwork.h"
#include "Player/Controller/PCController_GamePlay.h"

ADeathTriggerActor::ADeathTriggerActor()
{
	PrimaryActorTick.bCanEverTick = false;
	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = TriggerBox;

	TriggerBox->SetBoxExtent(FVector(100.f, 100.f, 50.f));
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ADeathTriggerActor::OnOverlapBegin);
}

void ADeathTriggerActor::BeginPlay()
{
	Super::BeginPlay();
}

void ADeathTriggerActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
		{
			//if (Player->HasAuthority())
			//{
			//	Player->ServerProcessDeath(RespawnLocation);
			//}
			// 서버에서 목숨 관리 코드 변경
			APCController_GamePlay* PC = Player->GetController<APCController_GamePlay>();
			if (IsValid(PC))
			{
				PC->OnPlayerFalltoDeath();
			}
		}


	}
}
