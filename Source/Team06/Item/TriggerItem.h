#pragma once

#include "CoreMinimal.h"
#include "Item/ItemBase.h"
#include "GameFramework/Actor.h"
#include "TriggerItem.generated.h"

class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TEAM06_API ATriggerItem : public AItemBase
{
	GENERATED_BODY()
	
public:	
	ATriggerItem();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	// 트리거 실행 (블루프린트 오버라이드 가능)
	UFUNCTION(BlueprintNativeEvent, Category = "Trigger")
	void TriggerEffect(AActor* OverlappedActor);
	virtual void TriggerEffect_Implementation(AActor* OverlappedActor);

	// 트리거 발생 시 자동 제거 여부
	UPROPERTY(EditAnywhere, Category = "Trigger")
	bool bAutoDestroyOnTrigger = true;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger")
	USphereComponent* TriggerArea;

	UPROPERTY(EditAnywhere, Category = "Debug")
	FString DebugMessage;
};
