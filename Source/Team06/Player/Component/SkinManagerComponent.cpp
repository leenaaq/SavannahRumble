// CharacterSkinRow에 원하는 이름과 메시를 넣고 아래 함수 호출
// 사용 -> SkinManagerComponent->ApplySkinByName(FName("행이름"));

#include "Player/Component/SkinManagerComponent.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Net/UnrealNetwork.h"

USkinManagerComponent::USkinManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void USkinManagerComponent::ApplySkinByName(FName SkinRowName)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_ApplySkinByName(SkinRowName);
	}
	else
	{
		Multicast_ApplySkinByName(SkinRowName);
	}
}

void USkinManagerComponent::Server_ApplySkinByName_Implementation(FName SkinRowName)
{
	Multicast_ApplySkinByName(SkinRowName);
}

bool USkinManagerComponent::Server_ApplySkinByName_Validate(FName SkinRowName)
{
	return true;
}

void USkinManagerComponent::Multicast_ApplySkinByName_Implementation(FName SkinRowName)
{
	InternalApplySkin(SkinRowName);
}

void USkinManagerComponent::InternalApplySkin(FName SkinRowName)
{
	if (!SkinMeshDataTable)
	{
		UE_LOG(LogTemp, Error, TEXT("[SkinManager] SkinMeshDataTable이 설정"));
		return;
	}

	static const FString ContextString(TEXT("SkinLookup"));
	const FSkinMeshRow* Row = SkinMeshDataTable->FindRow<FSkinMeshRow>(SkinRowName, ContextString);

	if (!Row)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkinManager] 행 '%s' 없음"), *SkinRowName.ToString());
		return;
	}
	if (!Row->SkeletalMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("[SkinManager] 행 '%s'의 메시 없음"), *SkinRowName.ToString());
		return;
	}

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("[SkinManager] Owner가 캐릭터가 아님"));
		return;
	}

	USkeletalMeshComponent* MeshComp = OwnerCharacter->GetMesh();
	if (!MeshComp)
	{
		UE_LOG(LogTemp, Error, TEXT("[SkinManager] MeshComponent 없음"));
		return;
	}

	MeshComp->SetSkeletalMesh(Row->SkeletalMesh);
	//UE_LOG(LogTemp, Log, TEXT("[SkinManager] '%s' 적용"), *SkinRowName.ToString());
}