#include "Player/Component/ItemManagerComponent.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "../Player/PlayerBase.h"
#include "Components/SkeletalMeshComponent.h"

void UItemManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    InitializeItemMeshMap();
    ValidateEssentialReferences();
}

void UItemManagerComponent::InitializeItemMeshMap()
{
    if (!ItemDataTable) return;

    static const FString Context = TEXT("InitMeshMap");

    for (auto RowName : ItemDataTable->GetRowNames())
    {
        if (const FEquipItemDataRow* Row = ItemDataTable->FindRow<FEquipItemDataRow>(RowName, Context))
        {
            UStaticMeshComponent* NewMesh = NewObject<UStaticMeshComponent>(GetOwner());
            if (NewMesh)
            {
                NewMesh->RegisterComponent();
                NewMesh->AttachToComponent(Cast<ACharacter>(GetOwner())->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("hand_r_socket"));
                NewMesh->SetStaticMesh(Row->StaticMesh);
                NewMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
                NewMesh->SetVisibility(false);

                ItemMeshMap.Add(Row->ItemName, NewMesh);
            }
        }
    }
}

void UItemManagerComponent::ValidateEssentialReferences()
{
    if (ItemDataTable == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] UItemManagerComponent : ItemDataTable is not assigned!"));
    }

    if (ItemMeshMap.Num() == 0)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] UItemManagerComponent : ItemMeshMap is empty!"));
    }

    AActor* OwnerActor = GetOwner();
    if (OwnerActor == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("[CHECK] UItemManagerComponent : Owner is invalid!"));
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
        //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: Owner is not a PlayerBase."));
        return;
    }

    if (ItemDataTable && NewItemName != NAME_None)
    {
        static const FString ContextString(TEXT("ItemManagerContext"));
        FEquipItemDataRow* FoundRow = ItemDataTable->FindRow<FEquipItemDataRow>(NewItemName, ContextString, true);
        if (FoundRow)
        {
            if (FoundRow->StaticMesh)
            {
                //UE_LOG(LogTemp, Log, TEXT("UpdateItemVisibility: Found item %s, mapping StaticMesh: %s."), *NewItemName.ToString(), *FoundRow->StaticMesh->GetName());
                Player->SetEquipItemMeshStatic(FoundRow->StaticMesh);
            }
            else
            {
                //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: Found item %s but StaticMesh is nullptr."), *NewItemName.ToString());
                Player->SetEquipItemMeshStatic(nullptr);
            }
        }
        else
        {
            //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: No row found for item %s."), *NewItemName.ToString());
            Player->SetEquipItemMeshStatic(nullptr);
        }
    }
    else
    {
        //UE_LOG(LogTemp, Warning, TEXT("UpdateItemVisibility: ItemDataTable is null or NewItemName is NAME None."));
        Player->SetEquipItemMeshStatic(nullptr);
    }
}
