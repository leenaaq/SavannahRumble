#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_PlayerNameText.generated.h"

class UTextBlock;

UCLASS()
class TEAM06_API UUW_PlayerNameText : public UUserWidget
{
	GENERATED_BODY()

public:
	UUW_PlayerNameText(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	AActor* GetOwningActor() const { return OwningActor; }

	void SetOwningActor(AActor* InOwningActor) { OwningActor = InOwningActor; }

	void UpdatePlayerName();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> PlayerNameText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AActor> OwningActor;
};