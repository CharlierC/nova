// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONMP_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
public:
	ABatteryPickup();
	
	// virtual void WasCollected_Implementation() override;
	virtual void PickUpBy(APawn* Pawn) override;

	UFUNCTION(BlueprintPure, Category="Power")
	float GetPower();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(Replicated,EditAnywhere,BlueprintReadWrite,Category="Power",meta=(BlueprintProtected="true"))
	float BatteryPower;

	
};
