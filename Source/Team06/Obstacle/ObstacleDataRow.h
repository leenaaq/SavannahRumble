#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ObstacleDataRow.generated.h"

USTRUCT(BlueprintType)
struct FObstacleDataRow : public FTableRowBase
{
    GENERATED_BODY();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ObstacleType = "None";

    // ------------------------------회전문------------------------------
    // 회전 시작 전 지연 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DoorCycle = 0.0f;

    // 회전 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DoorRotationSpeed = 0.0f;

    // 목표 회전 각도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator DoorRotationAngle = FRotator::ZeroRotator;


    // ------------------------------미는 장애물------------------------------
    // 이동 거리
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PushingMoveDistance = 0.0f;

    // 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PushingSpeed = 0.0f;

    // 이동 전환 주기
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PushingMoveCycle = 0.0f;

    // 왕복 후 재사용 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PushingPauseAfterRoundTrip = 0.0f;

    // 유지 시간 (파괴, 0 기입 시 파괴X)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float PushingDestroyAfterSeconds = 0.0f;


    // ------------------------------낙하 땅------------------------------
    // 낙하 지연 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FallDelayTime = 0.0f;

    // 낙하 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FallImpulse = 0.0f;

    // 파괴 시간
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float FallDestroyAfter = 0.0f;

    // ------------------------------눈보라------------------------------
    // 눈보라 데미지 주기(초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BlizzardTickInterval = 0.0f;

    // 눈보라 데미지
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float BlizzardDamage = 0.0f;

    // ------------------------------모닥불------------------------------
    // 모닥불 힐 주기(초)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealInterval = 0.0f;

    // 모닥불 힐
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HealAmount = 0.0f;

};
