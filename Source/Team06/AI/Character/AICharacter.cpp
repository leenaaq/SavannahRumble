// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character/AICharacter.h"
#include "AI/System/AIC_Enemy.h"

AAICharacter::AAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIC_Enemy::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}
