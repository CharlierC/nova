// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.h"
#include "SpawnVolume.generated.h"



UCLASS()
class THIRDPERSONMP_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE class UBoxComponent* GetWhereToSpawn() const {return WhereToSpawn;}
	
	UFUNCTION(BlueprintPure, Category="Spawning")
	FVector GetRandomPointInVolume();

	UFUNCTION(BlueprintCallable, Category="Spawning")
	void SetSpawningActivate(bool bShouldSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	void SpawnPickup();

protected:
	UPROPERTY(EditAnywhere, Category="Spawning")
	TSubclassOf<class APickup> WhatToSpawn;

	FTimerHandle SpawnTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float SpawnDelayRangeLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Spawning")
	float SpawnDelayRangeHigh;


private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category="Spawning",meta=(AllowPrivateAccess="true"))
	class UBoxComponent* WhereToSpawn;

	float SpawnDelay;

};
