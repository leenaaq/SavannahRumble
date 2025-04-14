// GrabActor.h

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GrabActor.generated.h"

class UPhysicsConstraintComponent;
class UStaticMeshComponent;
class UMaterialInterface;
class UMaterialInstanceDynamic;

UCLASS()
class TEAM06_API AGrabActor : public AActor
{
    GENERATED_BODY()

public:
    AGrabActor();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    void ActivateGrab();
    void ReleaseGrab();

    UFUNCTION(NetMulticast, Reliable)
    void MulticastTryGrabBone(const FVector& HandLocation);

    UPROPERTY(Replicated)
    UPhysicsConstraintComponent* ConstraintComp;

private:
    // 원래 대상 잡기 시도 함수 (원하는 경우 추후 실제 Grab 실행 시 호출)
    void TryGrabBone(const FVector& HandLocation);

private:

    UPROPERTY(EditAnywhere, Category = "Grab")
    float TraceRadius = 25.f;

    UPROPERTY()
    USkeletalMeshComponent * LastTargetMesh;

    bool bIsActive;

    // [추가] 소켓 범위를 표시할 컴포넌트
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* RangeIndicator;

    // [추가] 표시용 머티리얼의 베이스 (에디터에서 할당)
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Grab", meta = (AllowPrivateAccess = "true"))
    UMaterialInterface* RangeIndicatorBaseMaterial;

    // [추가] 동적 머티리얼 인스턴스 (실행 중 색상 변경)
    UMaterialInstanceDynamic* RangeIndicatorMaterial;
};
