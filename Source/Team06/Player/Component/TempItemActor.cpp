#include "Player/Component/TempItemActor.h"
#include "Components/SphereComponent.h"
#include "../Player/PlayerBase.h"
#include "Net/UnrealNetwork.h"

ATempItemActor::ATempItemActor()
{
    PrimaryActorTick.bCanEverTick = false;

    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    RootComponent = SphereComp;
    SphereComp->InitSphereRadius(50.f);

    SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ATempItemActor::OnSphereOverlap);

    bReplicates = true;

    ItemName = "TestSword1";
}

void ATempItemActor::BeginPlay()
{
    Super::BeginPlay();
}

void ATempItemActor::OnSphereOverlap(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{

    if (!HasAuthority())
    {
        return;
    }

    if (APlayerBase* Player = Cast<APlayerBase>(OtherActor))
    {
        Player->ServerSetEquippedItemName(ItemName);

        Destroy();
    }
}
