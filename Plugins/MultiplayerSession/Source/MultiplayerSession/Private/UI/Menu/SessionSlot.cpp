// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SessionSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystemPlugin.h"

void USessionSlot::SetResult(const FOnlineSessionSearchResult& SessionSearchResult)
{
	/*AddToViewport();*/
	/*SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;*/


	/*UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		OnlineSubsystemPlugin = GameInstance->GetSubsystem<UOnlineSubsystemPlugin>();
	}*/

	//Btn_JoinServer->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);
	SearchResult = SessionSearchResult;
	//Txt_ServerName->SetText(Session);

	GEngine->AddOnScreenDebugMessage(
		-1,
		15.f,
		FColor::Red,
		TEXT("Slot is created")
	);
}

void USessionSlot::OnJoinButtonClicked()
{
	//OnlineSubsystemPlugin->JoinSession(SearchResult);
}