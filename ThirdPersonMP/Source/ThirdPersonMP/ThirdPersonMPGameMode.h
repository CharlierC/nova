// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
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
	float GetPowerToWin();


private:
	void DrainPowerOverTime();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup",meta=(BlueprintProtected="true"))
	float DecayRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Pickup")
	float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;

	UPROPERTY(BlueprintReadWrite, Category="Pickup")
	float PowerToWin;

	

	
	
};



