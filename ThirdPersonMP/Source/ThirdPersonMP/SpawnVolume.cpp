// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"

#include "Pickup.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/BoxComponent.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	if (GetLocalRole() == ROLE_Authority)
	{
		WhereToSpawn = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolumn"));
		RootComponent = WhereToSpawn;

		SpawnDelayRangeHigh = 1.0f;
		SpawnDelayRangeLow = 0.2f;
	}

}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	SpawnDelay = FMath::RandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer,this,&ASpawnVolume::SpawnPickup,SpawnDelay,false);
}

void ASpawnVolume::SpawnPickup()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		if (UWorld* const World = GetWorld())
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector SpawnLocation = GetRandomPointInVolume();
			FRotator SpawnRotation(FMath::Rand() * 360.f, FMath::Rand() * 360.f, FMath::Rand() * 360.f);

			APickup* const SpawnPickup = World->SpawnActor<APickup>(WhatToSpawn, SpawnLocation, SpawnRotation, SpawnParams);

			SpawnDelay = FMath::RandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer,this,&ASpawnVolume::SpawnPickup,SpawnDelay,false);
		}
	}
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector ASpawnVolume::GetRandomPointInVolume()
{
	if (WhereToSpawn != NULL)
	{
		return UKismetMathLibrary::RandomPointInBoundingBox(WhereToSpawn->Bounds.Origin, WhereToSpawn->Bounds.BoxExtent);
	}
	return FVector();
}

