// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MPGameStateBase.h"
#include "ThirdPersonMPGameMode.generated.h"

UCLASS(minimalapi)
class AThirdPersonMPGameMode : public AGameModeBase
{
private:
	GENERATED_BODY()

public:
	AThirdPersonMPGameMode();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure,Category="Pickup")
	float GetDecayRate();

	UFUNCTION(BlueprintPure,Category="Pickup")
	float GetPowerDrainDelay();

	UFUNCTION(BlueprintPure,Category="Pickup")
	float GetPowerToWinMultiplier();

	UFUNCTION(BlueprintCallable, Category="SeamlessTravel")
	void SeamlessTravelTo(FString URL);


private:
	void DrainPowerOverTime();

	void HandleNewState(EBatteryPlayState NewState);

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup",meta=(BlueprintProtected="true"))
	float DecayRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup")
	float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite, Category="Pickup",meta=(BlueprintProtected="ture"))
	float PowerToWinMultiplier;

	int32 DeadPlayerCount;
	
	TArray<class ASpawnVolume*> SpawnVolumeActors;

	// virtual void PostLogin(APlayerController* NewPlayer) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Room")
	int32 ReadyPlayer;
	
};



