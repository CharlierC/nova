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

	UFUNCTION(BlueprintPure,Category="Power")
	float GetDecayRate();

	UFUNCTION(BlueprintPure,Category="Power")
	float GetPowerDrainDelay();

private:
	void DrainPowerOverTime();

protected:

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Power",meta=(BlueprintProtected="true"))
	float DecayRate;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Power")
	float PowerDrainDelay;

	FTimerHandle PowerDrainTimer;

	

	
	
};



