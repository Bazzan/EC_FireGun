// Copyright Epic Games, Inc. All Rights Reserved.

#include "Characters/EC_MinionCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

AEC_MinionCharacter::AEC_MinionCharacter()
{
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
}

