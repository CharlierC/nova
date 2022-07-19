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

	UFUNCTION(BlueprintNativeEvent, Category="Pickup")
	void  WasCollected();
	virtual void WasCollected_Implementation();

	UFUNCTION()
	virtual void OnRep_IsActive();
	
	UFUNCTION(BlueprintAuthorityOnly, Category="Pickup")
	virtual void PickUpBy(APawn* Pawn);

private:

	UFUNCTION(NetMulticast, Unreliable)
	void ClientOnPickUpBy(APawn* Pawn);
	
protected:
	
	UPROPERTY(ReplicatedUsing=OnRep_IsActive)
	bool bIsActive;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Pickup")
	APawn *PickupInstigator;
};
