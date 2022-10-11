// Fill out your copyright notice in the Description page of Project Settings.


#include "OnlineSubsystemPlugin.h"
//#include "OnlineSubsystemSteam.h"

UOnlineSubsystemPlugin::UOnlineSubsystemPlugin()
{
	Subsystem = IOnlineSubsystem::Get();
	if (Subsystem)
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}
	SessionSearch = MakeShareable(new FOnlineSessionSearch);
}

void UOnlineSubsystemPlugin::OnCreateSessionComplete(FName SessionName, bool bSucceded)
{
	SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionDelegateHandle);
	if (bSucceded)
	{
		GetWorld()->ServerTravel("/Game/CPlusProject/Map/LobbyMap?listen");
	}
}

void UOnlineSubsystemPlugin::OnFindSessionsComplete(bool bSucceded)
{
	SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsDelegateHandle);
	if (!bSucceded)
	{
		UE_LOG(LogTemp, Warning, TEXT("Search is not suceeded"));
		UpdateSessionInfo.Broadcast(TArray<FOnlineSessionSearchResult>());
		return;
	}
	UpdateSessionInfo.Broadcast(SessionSearch->SearchResults);
}

void UOnlineSubsystemPlugin::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionDelegateHandle);
	APlayerController* PController = GetGameInstance()->GetFirstLocalPlayerController();
	if (PController)
	{
		FString JoinAddress;
		if (SessionInterface->GetResolvedConnectString(SessionName, JoinAddress))
		{
			PController->ClientTravel(JoinAddress, ETravelType::TRAVEL_Absolute);
		}
	}
}

void UOnlineSubsystemPlugin::OnDestroySessionComplete(FName SessionName, bool bSucceded)
{
}

void UOnlineSubsystemPlugin::OnStartSessionComplete(FName SessionName, bool bSucceded)
{
}

void UOnlineSubsystemPlugin::CreateSession(FString SessionName, bool bLAN)
{	
	if (!Subsystem)
	{
		return;
	}

	TSharedRef<FOnlineSessionSettings> SessionSettings = MakeShared<FOnlineSessionSettings>();
	SessionSettings->NumPublicConnections = 5;
	SessionSettings->bIsLANMatch = bLAN;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = !bLAN;
	SessionSettings->bAllowInvites = true;
	SessionSettings->Set(FName("SERVER_NAME_KEY"), SessionName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	this->OnCreateSessionDelegateHandle =
		SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(FOnCreateSessionComplete::FDelegate::CreateUObject(
			this,
			&UOnlineSubsystemPlugin::OnCreateSessionComplete));

	const APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(LocalPlayerController) && LocalPlayerController->IsLocalPlayerController())
	{
		const ULocalPlayer* LocalPlayer = LocalPlayerController->GetLocalPlayer();

		if (IsValid(LocalPlayer))
		{
			if (LocalPlayer->GetPreferredUniqueNetId() != nullptr)
			{
				SessionInterface->CreateSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId().ToSharedRef().Get(), *SessionName, *SessionSettings);
			}
		}
	}
}

void UOnlineSubsystemPlugin::FindSession()
{
	if (!Subsystem)
	{
		return;
	}

	SessionSearch->bIsLanQuery =  true;
	SessionSearch->MaxSearchResults = 500;
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
	
	this->OnFindSessionsDelegateHandle =
		SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FOnFindSessionsComplete::FDelegate::CreateUObject(
			this,
			&UOnlineSubsystemPlugin::OnFindSessionsComplete));

	const APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(LocalPlayerController) && LocalPlayerController->IsLocalPlayerController())
	{
		const ULocalPlayer* LocalPlayer = LocalPlayerController->GetLocalPlayer();

		if (IsValid(LocalPlayer))
		{
			if (LocalPlayer->GetPreferredUniqueNetId() != nullptr)
			{
				SessionInterface->FindSessions(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId().ToSharedRef().Get(), SessionSearch.ToSharedRef());
			}
		}
	}
}

void UOnlineSubsystemPlugin::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing"));
		return;
	}

	FString SessionName = "Empty session name";
	SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME_KEY"), SessionName);
	
	this->OnJoinSessionDelegateHandle =
		SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(FOnJoinSessionComplete::FDelegate::CreateUObject(
			this,
			&UOnlineSubsystemPlugin::OnJoinSessionComplete));

	const APlayerController* LocalPlayerController = GetWorld()->GetFirstPlayerController();
	if (IsValid(LocalPlayerController) && LocalPlayerController->IsLocalPlayerController())
	{
		const ULocalPlayer* LocalPlayer = LocalPlayerController->GetLocalPlayer();

		if (IsValid(LocalPlayer))
		{
			if (LocalPlayer->GetPreferredUniqueNetId() != nullptr)
			{
				SessionInterface->JoinSession(LocalPlayer->GetPreferredUniqueNetId().GetUniqueNetId().ToSharedRef().Get(), *SessionName, SearchResult);
			}
		}
	}
}

void UOnlineSubsystemPlugin::DestroySession()
{
}

void UOnlineSubsystemPlugin::StartSession()
{
}