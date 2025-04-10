// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/Controller/PlayerCharacterController.h"
#include "PCController_Title.generated.h"

/**
 * 
 */
UCLASS()
class TEAM06_API APCController_Title : public APlayerCharacterController
{
	GENERATED_BODY()



public:
	virtual void BeginPlay() override;

	void JoinServer(const FString& InIPAddress, const FString& InUserName);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = ASUIPlayerController, Meta = (AllowPrivateAccess))
	TSubclassOf<UUserWidget> TitleUIWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = ASUIPlayerController, Meta = (AllowPrivateAccess))
	TObjectPtr<UUserWidget> TitleUIWidgetClassInstance;
};
