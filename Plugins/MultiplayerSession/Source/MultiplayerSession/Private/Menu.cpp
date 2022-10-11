// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/EditableTextBox.h"
#include "Components/CheckBox.h"
#include "OnlineSubsystemPlugin.h"
#include "UI/Menu/SessionSlot.h"

UMenu::UMenu(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer)
{
	/** Load class from asset in Content Browser of the UserWidget we created (USessionSlot) */
	static ConstructorHelpers::FClassFinder<USessionSlot> WidgetAsset(TEXT("/MultiplayerSession/W_ServerSlot"));
	if (WidgetAsset.Succeeded())
	{
		/** Assign the class of the loaded asset to the WigetClass variable, which is a "subclass" of USessionSlot : Which our asset class is */
		SlotClass = WidgetAsset.Class;
	}
}

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
	Btn_Join->OnClicked.AddDynamic(this, &ThisClass::OnJoinClicked);
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
	UE_LOG(LogTemp, Warning, TEXT("Found %i sessions"), SessionSearchResult.Num());

	if (SessionSearchResult.Num() > 0)
	{
		for (auto Result : SessionSearchResult)
		{
			FString SessionName = "Empty session name";
			Result.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), SessionName);

			if (SlotClass)
			{
				Slot = CreateWidget<USessionSlot>(GetWorld(), SlotClass);
				if (Slot)
				{
					Slot->AddToViewport();
					ScrlBox_ServerList->AddChild(Slot);
					Slot->SetResult(SessionName, Result);
				}
			}
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

	bool IsLAN = CheckBox_IsLan->IsChecked();
	FString SessionName;
	SessionName = TxtBox_SessionName->GetText().ToString();

	OnlineSubsystemPlugin->CreateSession(SessionName, IsLAN);
}

void UMenu::OnRefreshClicked()
{
	Btn_Refresh->SetIsEnabled(false);
	OnlineSubsystemPlugin->UpdateSessionInfo.AddUObject(this, &ThisClass::OnSessionUpdated);
	OnlineSubsystemPlugin->FindSession();
}

void UMenu::OnJoinClicked()
{
	if (TxtBox_IP->GetText().IsEmpty())
	{
		return;
	}

	FString IPAddres;
	IPAddres = TxtBox_IP->GetText().ToString();
	APlayerController* PController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PController)
	{
		PController->ClientTravel(IPAddres, ETravelType::TRAVEL_Absolute);
	}
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
