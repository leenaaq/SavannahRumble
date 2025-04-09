#include "Item/ItemBase.h"
#include "NiagaraFunctionLibrary.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

AItemBase::AItemBase()
{
 	
	PrimaryActorTick.bCanEverTick = false;

}


void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AItemBase::PlayItemEffects(FVector Location)
{
	if (NiagaraEffect)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraEffect, Location);
	}

	if (LegacyEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LegacyEffect, Location);
	}

	if (UseSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), UseSound, Location);
	}
}
