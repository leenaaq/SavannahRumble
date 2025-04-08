#include "Player/Component/ItemManagerComponent.h"
//#include "Net/UnrealNetwork.h"
//#include "Engine/DataTable.h"
//#include "GameFramework/Actor.h"
//#include "../Player/PlayerBase.h"
//#include "Components/SkeletalMeshComponent.h"

//UItemManagerComponent::UItemManagerComponent()
//{
//    SetIsReplicatedByDefault(true);
//    EquippedItemName = NAME_None;
//}
//
//void UItemManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
//{
//    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//    DOREPLIFETIME(UItemManagerComponent, EquippedItemName);
//}
//
//void UItemManagerComponent::OnRep_EquippedItemName()
//{
//    UpdateItemVisibility(EquippedItemName);
//}
//
//void UItemManagerComponent::ServerEquipItem_Implementation(FName NewItemName)
//{
//    if (ItemDataTable && NewItemName != NAME_None)
//    {
//        static const FString ContextString(TEXT("ItemManagerContext"));
//        FEquipItemDataRow* FoundRow = ItemDataTable->FindRow<FEquipItemDataRow>(NewItemName, ContextString, true);
//        if (!FoundRow)
//        {
//            return;
//        }
//    }
//
//    EquippedItemName = NewItemName;
//
//    UpdateItemVisibility(EquippedItemName);
//}
//
//bool UItemManagerComponent::ServerEquipItem_Validate(FName NewItemName)
//{
//    return true;
//}
//
//void UItemManagerComponent::UpdateItemVisibility(FName NewItemName)
//{
//    APlayerBase* Player = Cast<APlayerBase>(GetOwner());
//    if (!Player)
//    {
//        //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: Owner is not a PlayerBase."));
//        return;
//    }
//
//    if (ItemDataTable && NewItemName != NAME_None)
//    {
//        static const FString ContextString(TEXT("ItemManagerContext"));
//        FEquipItemDataRow* FoundRow = ItemDataTable->FindRow<FEquipItemDataRow>(NewItemName, ContextString, true);
//        if (FoundRow)
//        {
//            if (FoundRow->StaticMesh)
//            {
//                //UE_LOG(LogTemp, Log, TEXT("UpdateItemVisibility: Found item %s, mapping StaticMesh: %s."), *NewItemName.ToString(), *FoundRow->StaticMesh->GetName());
//                Player->SetEquipItemMeshStatic(FoundRow->StaticMesh);
//            }
//            else
//            {
//                //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: Found item %s but StaticMesh is nullptr."), *NewItemName.ToString());
//                Player->SetEquipItemMeshStatic(nullptr);
//            }
//        }
//        else
//        {
//            //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: No row found for item %s."), *NewItemName.ToString());
//            Player->SetEquipItemMeshStatic(nullptr);
//        }
//    }
//    else
//    {
//        //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: ItemDataTable is null or NewItemName is NAME None."));
//        Player->SetEquipItemMeshStatic(nullptr);
//    }
//}
