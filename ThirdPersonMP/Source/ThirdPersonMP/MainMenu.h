// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class THIRDPERSONMP_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonCreate;
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonJoin;
	UPROPERTY(meta=(BindWidget))
	UButton* ButtonSearch;

	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnclickCreate();
	UFUNCTION()
	void OnclickJoin();
	UFUNCTION()
	void OnclickSearch();
	
};
