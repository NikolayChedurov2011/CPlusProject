// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "OnlineSubsystemPlugin.h"
#include "UI/Menu/SessionSlot.h"

void UMenu::ShowMenu()
{
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(LocalPlayerController) && LocalPlayerController->IsLocalPlayerController())
	{
		FInputModeUIOnly InputModeData;
		InputModeData.SetWidgetToFocus(TakeWidget());
		InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		LocalPlayerController->SetInputMode(InputModeData);
		LocalPlayerController->SetShowMouseCursor(true);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance) 
	{
		OnlineSubsystemPlugin = GameInstance->GetSubsystem<UOnlineSubsystemPlugin>();
	}

	Btn_Create->OnClicked.AddDynamic(this, &ThisClass::OnCreateClicked);
	Btn_Refresh->OnClicked.AddDynamic(this, &ThisClass::OnRefreshClicked);
}

//bool UMenu::Initialize()
//{
//	if (!Super::Initialize())
//	{
//		return false;
//	}
//
//	return true;
//}

void UMenu::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	MenuTearDown();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}

void UMenu::OnSessionUpdated(const TArray<FOnlineSessionSearchResult>& SessionSearchResult)
{
	ScrlBox_ServerList->ClearChildren();

	if (SessionSearchResult.Num() > 0)
	{
		for (auto Result : SessionSearchResult)
		{
			FString SessionName = "Empty session name";
			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), SessionName);
			USessionSlot* SessionSlot;
			SessionSlot->SetResult(Result);
			AddSlot(SessionName);
		}
	}
	OnlineSubsystemPlugin->UpdateSessionInfo.Clear();

	Btn_Refresh->SetIsEnabled(true);
}

void UMenu::OnCreateClicked()
{
	if (TxtBox_SessionName->GetText().IsEmpty())
	{
		return;
	}

	FString SessionName;
	SessionName = TxtBox_SessionName->GetText().ToString();

	OnlineSubsystemPlugin->CreateSession(SessionName);
}

void UMenu::OnRefreshClicked()
{
	Btn_Refresh->SetIsEnabled(false);
	OnlineSubsystemPlugin->UpdateSessionInfo.AddUObject(this, &ThisClass::OnSessionUpdated);
	OnlineSubsystemPlugin->FindSession();
}

void UMenu::MenuTearDown()
{
	RemoveFromParent();
	APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(LocalPlayerController) && LocalPlayerController->IsLocalPlayerController())
	{
		FInputModeGameOnly InputModeData;
		LocalPlayerController->SetInputMode(InputModeData);
		LocalPlayerController->SetShowMouseCursor(false);
	}
}
