#include "Player/Component/EquipItemMeshActor.h"
#include "Components/StaticMeshComponent.h"

AEquipItemMeshActor::AEquipItemMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void AEquipItemMeshActor::BeginPlay()
{
	Super::BeginPlay();

}

void AEquipItemMeshActor::UpdateMesh(UStaticMesh* NewMesh)
{
    if (!MeshComponent)
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipItemMeshActor.cpp : MeshComponent 확인"));
        return;
    }

    if (NewMesh)
    {
        MeshComponent->SetStaticMesh(NewMesh);
        MeshComponent->SetVisibility(true);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("EquipItemMeshActor.cpp : NewMesh 값이 null"));
        MeshComponent->SetVisibility(false);
    }
}