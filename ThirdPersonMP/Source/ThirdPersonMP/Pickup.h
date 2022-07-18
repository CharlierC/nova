// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "Pickup.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONMP_API APickup : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	APickup();

	UFUNCTION(BlueprintPure,Category="Pickup")
	bool IsActive();

	UFUNCTION(BlueprintCallable,Category="Pickup")
	void SetActive(bool NewPickupState);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	
protected:
	UFUNCTION()
	virtual void OnRep_IsActive();

	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void  WasCollected();
	virtual void WasCollected_Implementation();

protected:
	UPROPERTY(ReplicatedUsing=OnRep_IsActive)
	bool bIsActive;
};
