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
	ABatteryPickup();

	virtual void WasCollected_Implementation() override;
};
