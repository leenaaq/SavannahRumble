#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "ItemManagerComponent.generated.h"

//USTRUCT(BlueprintType)
//struct FEquipItemDataRow : public FTableRowBase
//{
//    GENERATED_BODY()
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    FName ItemName = "NONE";
//
//    UPROPERTY(EditAnywhere, BlueprintReadWrite)
//    UStaticMesh* StaticMesh = nullptr;
//};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TEAM06_API UItemManagerComponent : public UActorComponent
{
    GENERATED_BODY()

//public:
//    UItemManagerComponent();
//
//protected:
//    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
//
//    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemManager")
//    UDataTable* ItemDataTable;
//
//    UPROPERTY(ReplicatedUsing = OnRep_EquippedItemName, BlueprintReadOnly, Category = "ItemManager")
//    FName EquippedItemName;
//
//    UFUNCTION()
//    void OnRep_EquippedItemName();
//
//public:
//    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManager")
//    TMap<FName, USkeletalMeshComponent*> ItemMeshMap;
//
//    UFUNCTION(Server, Reliable, WithValidation)
//    void ServerEquipItem(FName NewItemName);
//
//    UFUNCTION()
//    void UpdateItemVisibility(FName NewItemName);
};
