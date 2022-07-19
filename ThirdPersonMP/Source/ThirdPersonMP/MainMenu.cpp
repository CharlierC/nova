// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Kismet/GameplayStatics.h"
//#include "CreateSessionCallbackProxy.h"
void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();
	if (ButtonCreate)
	{
		ButtonCreate->OnClicked.AddDynamic(this,&UMainMenu::OnclickCreate);
	}
	if (ButtonJoin)
	{
		ButtonJoin->OnClicked.AddDynamic(this,&UMainMenu::OnclickJoin);	
	}
	if (ButtonSearch)
	{
		ButtonSearch->OnClicked.AddDynamic(this,&UMainMenu::OnclickSearch);	
	}
}

void UMainMenu::OnclickCreate()
{
	//APlayerController* playercontroller = UGameplayStatics::GetPlayerController(GetWorld(),0);
	//UCreateSessionCallbackProxy::CreateSession(GetWorld(),playercontroller,2,true);
	// if(UCreateSessionCallbackProxy::CreateSession(GetWorld(),playercontroller,2,true))
	{
		// const FName MapName("room");
		// UGameplayStatics::OpenLevel(GetWorld(),MapName);
		// GEngine->AddOnScreenDebugMessage(-3, 0.5, FColor::Green,TEXT("创建房间成功"));
	}
	// else
	// {
	// 	GEngine->AddOnScreenDebugMessage(-3, 0.5, FColor::Green,TEXT("创建房间失败"));
	// }
}

void UMainMenu::OnclickJoin()
{
	GEngine->AddOnScreenDebugMessage(3, 0.5, FColor::Green,TEXT("Test22!!!"));
}

void UMainMenu::OnclickSearch()
{
	GEngine->AddOnScreenDebugMessage(-1, 0.5, FColor::Green,TEXT("Test333!!!"));
}
