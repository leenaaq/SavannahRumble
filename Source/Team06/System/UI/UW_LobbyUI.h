// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_LobbyUI.generated.h"

class APCController_GamePlay;

UCLASS()
class TEAM06_API UUW_LobbyUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UUW_LobbyUI(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	void Init(APCController_GamePlay* InOwner);

	UFUNCTION(BlueprintCallable)
	void UpdateReadyButtonState(bool bReady);
protected:
	virtual void NativeConstruct() override;


	UFUNCTION()
	void OnReadyButtonClicked();
	
	UFUNCTION()
	void OnAIAddButtonClicked();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> ReadyButton;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> AddAIButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> PigMeshChangeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> FoxMeshChangeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> WolfMeshChangeButton;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LobbyWidget, Meta = (AllowPrivateAccess, BindWidget))
	TObjectPtr<class UButton> DeerMeshChangeButton;

	UFUNCTION()
	void OnPigMeshChangeButtonClicked();

	UFUNCTION()
	void OnFoxMeshChangeButtonClicked();

	UFUNCTION()
	void OnWolfMeshChangeButtonClicked();

	UFUNCTION()
	void OnDeerMeshChangeButtonClicked();

	void DisableCharacterButtons();

	UPROPERTY()
	TObjectPtr<APCController_GamePlay> OwnerPlayerController;
};
