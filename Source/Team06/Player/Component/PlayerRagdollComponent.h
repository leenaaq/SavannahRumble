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

};
