#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemManagerComponent.generated.h"

UENUM(BlueprintType)
enum class EEquipItemType : uint8
{
    Invalid     UMETA(DisplayName = "None (Invalid)"),
    Melee       UMETA(DisplayName = "Melee"),
    Ranged      UMETA(DisplayName = "Ranged")
};

USTRUCT(BlueprintType)
struct FEquipItemDataRow : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ItemName = "NONE";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EEquipItemType ItemType = EEquipItemType::Invalid;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UStaticMesh* StaticMesh = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ProjectileBlueprint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProjectileSpeed = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ProjectileForce = 0.0f;
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
