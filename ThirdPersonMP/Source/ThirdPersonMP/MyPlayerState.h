// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONMP_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

	
public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	AMyPlayerState();
	UPROPERTY(Replicated,BlueprintReadWrite)
	bool bIsready;
	
};
