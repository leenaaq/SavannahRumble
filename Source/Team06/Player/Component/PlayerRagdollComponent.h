// PlayerRagdollComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"
#include "PlayerRagdollComponent.generated.h"

/**
 * 캐릭터 레그돌 물리 애니메이션 컴포넌트
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM06_API UPlayerRagdollComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UPlayerRagdollComponent();

    void ActivateRagdoll();

    void InitializePhysicalAnimation();

protected:
    virtual void BeginPlay() override;

private:
    // 일. 물리 애니메이션 컴포넌트
    UPROPERTY()
    UPhysicalAnimationComponent* PhysicalAnimation;

    // 이. 스켈레탈 메시
    UPROPERTY()
    USkeletalMeshComponent* SkeletalMesh;
};
