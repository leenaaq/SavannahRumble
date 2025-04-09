#pragma once

#include "CoreMinimal.h"
#include "EquipableItem.h"
#include "Bat_Item.generated.h"

UCLASS()
class TEAM06_API ABat_Item : public AEquipableItem
{
	GENERATED_BODY()

public:
	ABat_Item();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 아이템 사용 함수 (서버에서 실행)
	virtual void ServerUseItem(AActor* Target) override;
	virtual void ServerUseItem_Implementation(AActor* Target) override;
	virtual bool ServerUseItem_Validate(AActor* Target) override;  // 추가됨

	// 아이템 던지기
	virtual void ServerThrowItem(FVector Direction) override;
	virtual void ServerThrowItem_Implementation(FVector Direction) override;
	virtual bool ServerThrowItem_Validate(FVector Direction) override;  // 추가됨

	// 충돌 처리
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
