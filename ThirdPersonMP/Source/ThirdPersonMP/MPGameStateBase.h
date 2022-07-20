// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MPGameStateBase.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum EBatteryPlayState
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS()
class THIRDPERSONMP_API AMPGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMPGameStateBase();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_CurrentState();

	UFUNCTION(BlueprintPure, Category="Pickup")
	EBatteryPlayState GetCurrentState() const;

	void SetCurrentState(EBatteryPlayState NewState);

private:
	UPROPERTY(ReplicatedUsing=OnRep_CurrentState)
	TEnumAsByte <enum EBatteryPlayState> CurrentState;
public:

	UPROPERTY(Replicated, BlueprintReadWrite, Category="Pickup")
	float PowerToWin;

	UPROPERTY(Replicated, BlueprintReadWrite)
	FString WinningPlayerName;
};
