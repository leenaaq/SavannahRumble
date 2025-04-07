#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TriggerItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TEAM06_API ATriggerItem : public AActor
{
	GENERATED_BODY()
	
public:	
	ATriggerItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(Server, Reliable)
	void ServerTriggerEffect(AActor* OverlappedActor);
	void ServerTriggerEffect_Implementation(AActor* OverlappedActor);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 자식에서 오버라이드할 처리 함수
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
	void TriggerEffect(AActor* OverlappedActor);
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor);

	// 트리거 발생 시 자동 제거 여부
	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bAutoDestroyOnTrigger = true;

public:
	// 메쉬 설정
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMeshComponent* ItemMesh;

	// 트리거 충돌 범위
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	USphereComponent* TriggerArea;

	// 디버그용 로그 메시지
	UPROPERTY(EditAnywhere, Category = "Debug")
	FString DebugMessage;
};
