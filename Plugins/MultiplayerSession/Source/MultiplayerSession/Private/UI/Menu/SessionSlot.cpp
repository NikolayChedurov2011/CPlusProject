// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Menu/SessionSlot.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "OnlineSubsystemPlugin.h"

bool USessionSlot::Initialize()
{
	Super::Initialize();
	
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		OnlineSubsystemPlugin = GameInstance->GetSubsystem<UOnlineSubsystemPlugin>();
	}
	UE_LOG(LogTemp, Warning, TEXT("Bind to join button"));
	Btn_JoinServer->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClicked);

	return true;
}

void USessionSlot::SetResult(const FString SessionSearchName, const FOnlineSessionSearchResult& SessionSearchResult)
{
	SessionName = SessionSearchName;
	SearchResult = SessionSearchResult;
	Txt_ServerName->SetText(FText::FromString(SessionName));
}

void USessionSlot::OnJoinButtonClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Join clicked"));
	OnlineSubsystemPlugin->JoinSession(SearchResult);
}