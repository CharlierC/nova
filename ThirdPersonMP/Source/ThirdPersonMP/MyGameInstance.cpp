// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/MoviePlayer/Public/MoviePlayer.h"



UMyGameInstance::UMyGameInstance()
	:Super(),LoadingWidgetClass(nullptr),LoadingWidget(nullptr)
{

}

void UMyGameInstance::Init()
{
	Super::Init();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMyGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMyGameInstance::EndLoadingScreen);

	bIsGuest = true;
}

void UMyGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = true;
		
		LoadingWidget = CreateWidget<UUserWidget>(this, LoadingWidgetClass);
		
		TSharedPtr<SWidget> WidgetPtr = LoadingWidget->TakeWidget();
		
		LoadingScreen.WidgetLoadingScreen = WidgetPtr;
		
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}

		
}

void UMyGameInstance::EndLoadingScreen(UWorld* LoadedWorld)
{

	if (!IsRunningDedicatedServer())
	{
		if (LoadingWidget)
		{

			LoadingWidget->RemoveFromParent();

			LoadingWidget->MarkPendingKill();
		}
	}
}

