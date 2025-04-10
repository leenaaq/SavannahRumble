#pragma once

#include "Components/WidgetComponent.h"
#include "PlayerTextWidgetComponent.generated.h"

UCLASS()
class TEAM06_API UPlayerTextWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	virtual void InitWidget() override;
};