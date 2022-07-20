// Fill out your copyright notice in the Description page of Project Settings.


#include "MPGameStateBase.h"

#include "Net/UnrealNetwork.h"

AMPGameStateBase::AMPGameStateBase()
{
	CurrentState = EBatteryPlayState::EUnknown;
}

void AMPGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMPGameStateBase,PowerToWin);
	DOREPLIFETIME(AMPGameStateBase,CurrentState);
	DOREPLIFETIME(AMPGameStateBase,WinningPlayerName);
}

void AMPGameStateBase::OnRep_CurrentState()
{

}

EBatteryPlayState AMPGameStateBase::GetCurrentState() const
{
	return CurrentState;
}

void AMPGameStateBase::SetCurrentState(EBatteryPlayState NewState)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentState = NewState;
	}
}
