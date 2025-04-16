#pragma once

#include "GameFramework/PlayerState.h"
#include "PlayerCharacterState.generated.h"

/**
 * 
 */

UCLASS()
class TEAM06_API APlayerCharacterState : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	UPROPERTY(ReplicatedUsing = OnRep_PlayerSkinName, BlueprintReadOnly, Category = "Character|Skin")
	FName PlayerSkinName;

	UFUNCTION()
	void OnRep_PlayerSkinName();
};
