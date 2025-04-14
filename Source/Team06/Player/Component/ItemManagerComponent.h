#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipItemType : uint8
{
    Invalid     UMETA(DisplayName = "DEFAULT (Invalid)"),
    Melee       UMETA(DisplayName = "Melee"),
    Ranged      UMETA(DisplayName = "Ranged")
};

USTRUCT(BlueprintType)
struct FEquipItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName = "DEFAULT";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class AItemBase> ItemActorClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipItemType ItemType = EEquipItemType::Invalid;

    // 원거리 공격 시 스폰 아이템
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ProjectileBlueprint;

    // 발사체 초기 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProjectileSpeed = 0.0f;

    // 발사체 힘
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProjectileForce = 0.0f;

    // 들고 있는 아이템 메시
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* StaticMesh = nullptr;

    // 들고 있는 아이템 메시 스케일 조절
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector MeshScale = FVector(.0f, .0f, .0f);

    // 들고 있는 아이템 메시 위치 조절
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FVector MeshOffset = FVector::ZeroVector;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM06_API UItemManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UItemManagerComponent();

    virtual void BeginPlay() override;
    void ValidateEssentialReferences();

    void InitializeItemMeshMap();

    UFUNCTION(Server, Reliable, WithValidation)
    void ServerEquipItem(FName NewItemName);

    UFUNCTION()
    void UpdateItemVisibility(FName NewItemName);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION()
    void OnRep_EquippedItemName();

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManager")
    TMap<FName, UStaticMeshComponent*> ItemMeshMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemManager")
    UDataTable* ItemDataTable;

    UPROPERTY(ReplicatedUsing = OnRep_EquippedItemName, BlueprintReadOnly, Category = "ItemManager")
    FName EquippedItemName;
};
