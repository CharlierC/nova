// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonMPGameMode.h"

#include "Payne.h"
#include "ThirdPersonMPCharacter.h"
#include "UObject/ConstructorHelpers.h"


AThirdPersonMPGameMode::AThirdPersonMPGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/BP_Payne"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	DecayRate = 0.02f;

	PowerDrainDelay = 0.25f;
}

void AThirdPersonMPGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(PowerDrainTimer,this,&AThirdPersonMPGameMode::DrainPowerOverTime,PowerDrainDelay,true);
}

float AThirdPersonMPGameMode::GetDecayRate()
{
	return DecayRate;
}

float AThirdPersonMPGameMode::GetPowerDrainDelay()
{
	return PowerDrainDelay;
}

void AThirdPersonMPGameMode::DrainPowerOverTime()
{
	UWorld* World = GetWorld();
	check(World);
	for (FConstControllerIterator It = World->GetControllerIterator(); It ; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController> (*It))
		{
			if (APayne* character = Cast<APayne>(PlayerController->GetPawn()))
			{
				if (character->GetCurrentPower() > 0)
				{
					character->UpdatePower(-PowerDrainDelay*DecayRate*character->GetInitialPower());
				}
			}
		}
	}
}
