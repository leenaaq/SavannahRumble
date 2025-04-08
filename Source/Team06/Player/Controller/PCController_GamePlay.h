// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "PCController_GamePlay.generated.h"

class UUserWidget;

UCLASS()
class TEAM06_API APCController_GamePlay : public APlayerCharacterController
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
public:
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite)
	FText NotificationText;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> NotificationTextUIClass;
#pragma region Loading
public:
	UFUNCTION(Client, Reliable)
	void ShowGameLoadingWidget();
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UUserWidget> GameLoadingClass;
#pragma endregion


#pragma region LobbyReady
public:
	void OnHandleLobbyUI();

	// Ready 버튼 UI 신호를 서버에 전달
	UFUNCTION(Server, Reliable)
	void ServerSetReady();

	// UI 생성 및 추적
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UUserWidget> LobbyWidgetClass;

	UPROPERTY()
	TObjectPtr<class UUW_LobbyUI> LobbyWidget;  // 캐스팅을 위해 정확한 타입 사용

	// 레디 상태 (옵션: 상태 유지용)
	UPROPERTY(ReplicatedUsing = OnRep_bIsReady, VisibleAnywhere, BlueprintReadOnly)
	bool bIsReady = false;
	UFUNCTION()
	void OnRep_bIsReady();

#pragma region
};
