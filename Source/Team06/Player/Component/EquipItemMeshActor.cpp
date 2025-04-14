#include "Player/Component/EquipItemMeshActor.h"
#include "Components/StaticMeshComponent.h"
<<<<<<< HEAD
#include "Components/SceneComponent.h"
=======
>>>>>>> ItemNew

AEquipItemMeshActor::AEquipItemMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

<<<<<<< HEAD
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	RootComponent = RootComp;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComp);

	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	MeshComp->SetCollisionResponseToAllChannels(ECR_Ignore);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
	if (SphereMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(SphereMesh.Object);
	}
=======
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
>>>>>>> ItemNew
}

void AEquipItemMeshActor::BeginPlay()
{
	Super::BeginPlay();

<<<<<<< HEAD
	ValidateEssentialReferences();
}

void AEquipItemMeshActor::ValidateEssentialReferences() const
{
	if (RootComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[CHECK] AEquipItemMeshActor : RootComp is missing!"));
	}

	if (MeshComp == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[CHECK] AEquipItemMeshActor : MeshComp is missing!"));
	}
}

=======
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
>>>>>>> ItemNew
