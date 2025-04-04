#include "PlayerCharacterController.h"

void APlayerCharacterController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}
}