// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"
#include "Net/UnrealNetwork.h"

ABatteryPickup::ABatteryPickup()
{
	
	//bReplicateMovement = true;
	//SetReplicates(true);
	bReplicates = true;
	
	//SetReplicatingMovement(true);
	GetStaticMeshComponent()->SetMobility(EComponentMobility::Movable);
	GetStaticMeshComponent()->SetSimulatePhysics(true);

	BatteryPower = 200.f;
}

// 被收集
// void ABatteryPickup::WasCollected_Implementation()
// {
// 	Super::WasCollected_Implementation();
// 	Destroy();
// }

void ABatteryPickup::PickUpBy(APawn* Pawn)
{
	Super::PickUpBy(Pawn);
	if (GetLocalRole() == ROLE_Authority)
	{
		SetLifeSpan(2.0f);
	}
}

float ABatteryPickup::GetPower() 
{
	return BatteryPower;
}

void ABatteryPickup::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABatteryPickup,BatteryPower);
}
