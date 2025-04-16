#include "ReviveItem.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Player/Controller/PCController_GamePlay.h"

AReviveItem::AReviveItem()
{
    PrimaryActorTick.bCanEverTick = true;

    bReplicates = true;
    bAlwaysRelevant = true;

    RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootSceneComponent;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    MeshComponent->SetupAttachment(RootComponent);

    CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComponent->InitSphereRadius(100.f);
    CollisionComponent->SetupAttachment(RootComponent);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AReviveItem::OnOverlapBegin);
}

void AReviveItem::BeginPlay()
{
    Super::BeginPlay();
}

void AReviveItem::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!HasAuthority())
    {
        AddActorLocalRotation(RotationRate * DeltaTime);
    }
}

void AReviveItem::OnOverlapBegin(
    UPrimitiveComponent* OverlappedComp,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult
)
{
    if (!HasAuthority() || bUsed)
        return;
    bUsed = true;

    if (APawn* Pawn = Cast<APawn>(OtherActor))
    {
        if (APlayerController* PC = Cast<APlayerController>(Pawn->GetController()))
        {
            if (APCController_GamePlay* GP = Cast<APCController_GamePlay>(PC))
            {
                for (int32 i = 0; i < LifeAmount; ++i)
                {
                    GP->ServerApplyLifeItem();
                }
            }
        }
        Destroy();
    }
}

