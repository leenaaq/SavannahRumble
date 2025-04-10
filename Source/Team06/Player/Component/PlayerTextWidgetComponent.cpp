#include "PlayerTextWidgetComponent.h"
#include "System/UI/UW_PlayerNameText.h"

void UPlayerTextWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UUW_PlayerNameText* NameWidget = Cast<UUW_PlayerNameText>(GetWidget());
	if (IsValid(NameWidget) == true)
	{
		NameWidget->SetOwningActor(GetOwner());
	}
}