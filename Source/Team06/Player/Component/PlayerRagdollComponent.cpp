// PlayerRagdollComponent.cpp

#include "PlayerRagdollComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

UPlayerRagdollComponent::UPlayerRagdollComponent()
{
    PrimaryComponentTick.bCanEverTick = false;

    PhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PlayerRagdollPhysicalAnimation"));
}

void UPlayerRagdollComponent::BeginPlay()
{
    Super::BeginPlay();
    InitializePhysicalAnimation();
}



void UPlayerRagdollComponent::InitializePhysicalAnimation()
{
    if (ACharacter* OwnerChar = Cast<ACharacter>(GetOwner()))
    {
        SkeletalMesh = OwnerChar->GetMesh();
        if (!SkeletalMesh)
        {
            UE_LOG(LogTemp, Error, TEXT("PlayerRagdollComponent: SkeletalMesh is nullptr"));
            return;
        }

        PhysicalAnimation->SetSkeletalMeshComponent(SkeletalMesh);

        SkeletalMesh->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true, false);
        SkeletalMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
        SkeletalMesh->SetEnableGravity(true);
        SkeletalMesh->bBlendPhysics = true;

        FPhysicalAnimationData AnimData;
        AnimData.bIsLocalSimulation = true;
        AnimData.OrientationStrength = 5000.0f;
        AnimData.PositionStrength = 500.0f;
        AnimData.VelocityStrength = 100.0f;
        AnimData.AngularVelocityStrength = 100.0f;

        PhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(FName("pelvis"), AnimData, false);

        SkeletalMesh->SetSimulatePhysics(false);
    }
}