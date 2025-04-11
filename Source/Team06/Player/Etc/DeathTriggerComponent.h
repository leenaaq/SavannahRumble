#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DeathTriggerComponent.generated.h"

class UBoxComponent;

UCLASS()
class TEAM06_API ADeathTriggerActor : public AActor
{
	GENERATED_BODY()

public:
	ADeathTriggerActor();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* TriggerBox;

	// 오버랩 이벤트 함수
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RespawnLocation = FVector::ZeroVector;
};