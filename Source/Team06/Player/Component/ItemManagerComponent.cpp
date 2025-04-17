#include "Player/Component/ItemManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "../Player/PlayerBase.h"
#include "Components/SkeletalMeshComponent.h"
#include "Player/Component/EquipItemMeshActor.h"

void UItemManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    InitializeItemMeshMap();
    ValidateEssentialReferences();
}

void UItemManagerComponent::InitializeItemMeshMap()
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : ItemDataTable 값이 null"));
        return;
    }

    static const FString Context = TEXT("InitMeshMap");

    ACharacter* CharacterOwner = Cast<ACharacter>(GetOwner());
    if (!CharacterOwner)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : Owner 확인"));
        return;
    }

    for (auto RowName : ItemDataTable->GetRowNames())
    {
        const FEquipItemDataRow* Row = ItemDataTable->FindRow<FEquipItemDataRow>(RowName, Context);
        if (!Row)
        {
            UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : %s 행 확인"), *RowName.ToString());
            continue;
        }

        if (Row->ItemName.IsNone())
        {
            UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : %s ItemName 확인"), *RowName.ToString());
            continue;
        }
    }

    APlayerBase* PlayerBase = Cast<APlayerBase>(CharacterOwner);
    if (PlayerBase)
    {
        FName CurrentItem = PlayerBase->GetCurrentEquippedItemName();
        UpdateItemVisibility(CurrentItem);
    }
}

void UItemManagerComponent::ValidateEssentialReferences()
{
    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : ItemDataTable 확인"));
    }
    else
    {
        for (const auto& Elem : ItemMeshMap)
        {
            if (!Elem.Value)
            {
                UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : %s MeshComponent 값이 null"), *Elem.Key.ToString());
            }
        }
    }

    AActor* OwnerActor = GetOwner();
    if (!OwnerActor)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : Owner 확인"));
    }

    APlayerBase* PlayerBase = Cast<APlayerBase>(OwnerActor);
    if (!PlayerBase)
    {
        UE_LOG(LogTemp, Error, TEXT("ItemManagerComponent.cpp : Owner APlayerBase 확인"));
    }
}


UItemManagerComponent::UItemManagerComponent()
{
    SetIsReplicatedByDefault(true);
    EquippedItemName = NAME_None;
}

void UItemManagerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UItemManagerComponent, EquippedItemName);
}

void UItemManagerComponent::OnRep_EquippedItemName()
{
    UpdateItemVisibility(EquippedItemName);
}

void UItemManagerComponent::ServerEquipItem_Implementation(FName NewItemName)
{
    if (ItemDataTable && NewItemName != NAME_None)
    {
        static const FString ContextString(TEXT("ItemManagerContext"));
        FEquipItemDataRow* FoundRow = ItemDataTable->FindRow<FEquipItemDataRow>(NewItemName, ContextString, true);
        if (!FoundRow)
        {
            return;
        }
    }

    EquippedItemName = NewItemName;

    UpdateItemVisibility(EquippedItemName);
}

bool UItemManagerComponent::ServerEquipItem_Validate(FName NewItemName)
{
    return true;
}

void UItemManagerComponent::UpdateItemVisibility(FName NewItemName)
{
    APlayerBase* Player = Cast<APlayerBase>(GetOwner());
    if (!Player)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : Owner APlayerBase 확인"));
        return;
    }

    if (!ItemDataTable)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : ItemDataTable 값이 null"));
        return;
    }

    static const FString Context(TEXT("UpdateItemVisibility"));
    const FEquipItemDataRow* Row = ItemDataTable->FindRow<FEquipItemDataRow>(NewItemName, Context);
    if (!Row)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : %s 행 확인"), *NewItemName.ToString());
        return;
    }

    if (!Row->StaticMesh)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : %s StaticMesh 값이 null"), *NewItemName.ToString());
        return;
    }

    if (!Player->EquipItemMeshActorComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : EquipItemMeshActorComponent 확인"));
        return;
    }

    AEquipItemMeshActor* EquipActor = Cast<AEquipItemMeshActor>(Player->EquipItemMeshActorComponent->GetChildActor());
    if (!EquipActor)
    {
        UE_LOG(LogTemp, Warning, TEXT("ItemManagerComponent.cpp : EquipActor 확인"));
        return;
    }

    EquipActor->UpdateMesh(Row->StaticMesh);
    EquipActor->SetActorScale3D(Row->MeshScale);
    EquipActor->SetActorRelativeLocation(Row->MeshOffset);
}

const FEquipItemDataRow* UItemManagerComponent::GetItemDataRow(FName RowName) const
{
    if (!ItemDataTable) return nullptr;

    static const FString Context = TEXT("ItemDataRowContext");
    return ItemDataTable->FindRow<FEquipItemDataRow>(RowName, Context);
}
