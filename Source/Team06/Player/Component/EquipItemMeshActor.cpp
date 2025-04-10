#include "Player/Component/EquipItemMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

AEquipItemMeshActor::AEquipItemMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;

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
}

void AEquipItemMeshActor::BeginPlay()
{
	Super::BeginPlay();

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

