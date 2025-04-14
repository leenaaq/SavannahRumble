#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "SkinManagerComponent.generated.h"

USTRUCT(BlueprintType)
struct FSkinMeshRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* SkeletalMesh;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM06_API USkinManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USkinManagerComponent();

	// 데이터 테이블 설정
	UPROPERTY(EditAnywhere, Category = "Skin")
	UDataTable* SkinMeshDataTable;

	// 스킨 적용 (클라이언트/서버 모두 호출 가능)
	UFUNCTION(BlueprintCallable, Category = "Skin")
	void ApplySkinByName(FName SkinRowName);

protected:
	// 서버 요청
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ApplySkinByName(FName SkinRowName);

	// 모든 클라이언트 동기화
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ApplySkinByName(FName SkinRowName);

	// 실제 메시 적용 (내부에서만 사용)
	void InternalApplySkin(FName SkinRowName);
};