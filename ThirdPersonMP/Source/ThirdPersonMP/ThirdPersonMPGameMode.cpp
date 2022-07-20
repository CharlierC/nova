// Copyright Epic Games, Inc. All Rights Reserved.

#include "ThirdPersonMPGameMode.h"

#include "Payne.h"
#include "ThirdPersonMPCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/HUD.h"
#include "MPGameStateBase.h"
#include "SpawnVolume.h"
#include "Engine/Engine.h"
#include "Kismet/GameplayStatics.h"


AThirdPersonMPGameMode::AThirdPersonMPGameMode()
{
	// set default pawn class to our Blueprinted character
	// static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));

	GameStateClass = AMPGameStateBase::StaticClass();
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Character/BP_Payne"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDClass(TEXT("/Game/UI/BP_MPGameHUD"));
	if (PlayerHUDClass.Class != NULL)
	{
		HUDClass = PlayerHUDClass.Class;
	}
	
	DecayRate = 0.02f;
	PowerDrainDelay = 0.25f;
	PowerToWinMultiplier = 1.25f;
	DeadPlayerCount = 0;
}

void AThirdPersonMPGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	UWorld* World = GetWorld();
	check(World);
	AMPGameStateBase* MyGameState = Cast<AMPGameStateBase>(GameState);
	check(MyGameState);

	DeadPlayerCount = 0;
	MyGameState->PowerToWin = 3000.f;


	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World,ASpawnVolume::StaticClass(),FoundActors);
	for (auto Actor : FoundActors)
	{
		if (ASpawnVolume* tmp = Cast<ASpawnVolume>(Actor))
		{
			SpawnVolumeActors.AddUnique(tmp);
		}	
	}

	//MyGameState->SetCurrentState(EBatteryPlayState::EPlaying);
	HandleNewState(EPlaying);
	// for (FConstControllerIterator It = World->GetControllerIterator(); It ; ++It)
	// {
	// 	if (APlayerController* PlayerController = Cast<APlayerController> (*It))
	// 	{
	// 		if (APayne* character = Cast<APayne>(PlayerController->GetPawn()))
	// 		{
	// 			MyGameState->PowerToWin = character->GetInitialPower() * PowerToWinMultiplier;
	// 		}
	// 	}
	// }
}

float AThirdPersonMPGameMode::GetDecayRate()
{
	return DecayRate;
}

float AThirdPersonMPGameMode::GetPowerDrainDelay()
{
	return PowerDrainDelay;
}

float AThirdPersonMPGameMode::GetPowerToWinMultiplier()
{
	return PowerToWinMultiplier;
}

void AThirdPersonMPGameMode::DrainPowerOverTime()
{
	UWorld* World = GetWorld();
	check(World);
	AMPGameStateBase* MyGamestate = Cast<AMPGameStateBase>(GameState);
	for (FConstControllerIterator It = World->GetControllerIterator(); It ; ++It)
	{
		if (APlayerController* PlayerController = Cast<APlayerController> (*It))
		{
			if (APayne* character = Cast<APayne>(PlayerController->GetPawn()))
			{
				if (character->GetCurrentPower() >= MyGamestate->PowerToWin)
				{
					//MyGamestate->SetCurrentState(EBatteryPlayState::EWon);
					MyGamestate->WinningPlayerName = character->GetName();
					HandleNewState(EWon);
				}
				else if (character->GetCurrentPower() > 0)
				{
					character->UpdatePower(-PowerDrainDelay*DecayRate*character->GetInitialPower());
				}
				else
				{
					character->OnPlayerDeath();
					++DeadPlayerCount;
					if (DeadPlayerCount >= GetNumPlayers()) {
						//MyGamestate->SetCurrentState(EBatteryPlayState::EGameOver);
						HandleNewState(EGameOver);
					}
				} 
			}
		}
	}
}

void AThirdPersonMPGameMode::HandleNewState(EBatteryPlayState NewState)
{
	AMPGameStateBase* MyGameState = Cast<AMPGameStateBase>(GameState);
	check(MyGameState);
	if (MyGameState->GetCurrentState() != NewState)
	{
		MyGameState->SetCurrentState(NewState);
		switch(NewState)
		{
		case EBatteryPlayState::EPlaying:
			GetWorldTimerManager().SetTimer(PowerDrainTimer,this,&AThirdPersonMPGameMode::DrainPowerOverTime,PowerDrainDelay,true);
			for (auto s : SpawnVolumeActors)
			{
				s->SetSpawningActivate(true);
			}
			break;
		case EBatteryPlayState::EWon:
			GetWorldTimerManager().ClearTimer(PowerDrainTimer);
			for (auto s : SpawnVolumeActors)
			{
				s->SetSpawningActivate(false);
			}
			break;
		case EBatteryPlayState::EGameOver:
			GetWorldTimerManager().ClearTimer(PowerDrainTimer);
			for (auto s : SpawnVolumeActors)
			{
				s->SetSpawningActivate(false);
			}
			break;
		case EBatteryPlayState::EUnknown:
			break;
		}
	}
}
