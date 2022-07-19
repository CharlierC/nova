// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"

#include "Net/UnrealNetwork.h"

APickup::APickup()
{
	bReplicates = true;
	PrimaryActorTick.bCanEverTick = false;

	GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	if (GetLocalRole() == ROLE_Authority)
	{
		bIsActive = true;
	}
}

bool APickup::IsActive()
{
	return bIsActive;
}

void APickup::SetActive(bool NewPickupState)
{
	if (GetLocalRole()==ROLE_Authority)
	{
		bIsActive = NewPickupState;
	}
}

void APickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APickup,bIsActive);
}

void APickup::OnRep_IsActive()
{
}

void APickup::PickUpBy(APawn* Pawn)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		PickupInstigator = Pawn;
		ClientOnPickUpBy(Pawn);
	}
}

void APickup::ClientOnPickUpBy_Implementation(APawn* Pawn)
{
	PickupInstigator = Pawn;
	WasCollected();
}

void APickup::WasCollected_Implementation()
{
	UE_LOG(LogClass,Log,TEXT("APickup::WasCollected_Implementation() %s"),*GetName());
}
