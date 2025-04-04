#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Bat_Item.generated.h"

class UCapsuleComponent;
class UStaticMeshComponent;

UCLASS()
class TEAM06_API ABat_Item : public AItem
{
	GENERATED_BODY()

public:
	ABat_Item();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// 콜리전, 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UCapsuleComponent> BatCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> BatMesh;

	// 아이템 사용 함수 (서버에서 실행)
	virtual void ServerUseItem(AActor* Target) override;
	virtual void ServerUseItem_Implementation(AActor* Target) override;
	virtual bool ServerUseItem_Validate(AActor* Target) override;  // 추가됨

	// 아이템 던지기
	virtual void ServerThrowItem(FVector Direction) override;
	virtual void ServerThrowItem_Implementation(FVector Direction) override;
	virtual bool ServerThrowItem_Validate(FVector Direction) override;  // 추가됨

	// 충돌 처리 (맞은 캐릭터 쓰러지게 만들기)
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;


};
