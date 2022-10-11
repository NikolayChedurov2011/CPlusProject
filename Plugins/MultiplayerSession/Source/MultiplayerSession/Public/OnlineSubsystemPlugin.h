// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "OnlineSubsystemPlugin.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSessionInfo
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	FString ServerName;
	UPROPERTY(BlueprintReadOnly)
	int32 CurrentPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 MaxPlayers;
	UPROPERTY(BlueprintReadOnly)
	int32 SearchId;
};

DECLARE_MULTICAST_DELEGATE_OneParam(FOSSUpdateSessionsListDelegate, const TArray<FOnlineSessionSearchResult>& SessionSearchResult);

UCLASS()
class MULTIPLAYERSESSION_API UOnlineSubsystemPlugin : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	FOSSUpdateSessionsListDelegate UpdateSessionInfo;

	UOnlineSubsystemPlugin();

	void CreateSession(FString SessionName, bool bLAN);
	void FindSession();
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	UFUNCTION(BlueprintCallable)
	void DestroySession();
	UFUNCTION(BlueprintCallable)
	void StartSession();

protected:

	void OnCreateSessionComplete(FName SessionName, bool bSucceded);
	void OnFindSessionsComplete(bool bSucceded);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bSucceded);
	void OnStartSessionComplete(FName SessionName, bool bSucceded);

private:

	IOnlineSubsystem* Subsystem;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
	IOnlineSessionPtr SessionInterface;
	/*const APlayerController* LocalPlayerController;
	const ULocalPlayer* LocalPlayer;*/
	FOnCreateSessionCompleteDelegate OnCreateSessionDelegate;
	FDelegateHandle OnCreateSessionDelegateHandle;
	FOnFindSessionsCompleteDelegate OnFindSessionsDelegate;
	FDelegateHandle OnFindSessionsDelegateHandle;
	FOnJoinSessionCompleteDelegate OnJoinSessionDelegate;
	FDelegateHandle OnJoinSessionDelegateHandle;
	FOnDestroySessionCompleteDelegate OnDestroySessionDelegate;
	FDelegateHandle OnDestroySessionDelegateHandle;
	FOnStartSessionCompleteDelegate OnStartSessionDelegate;
	FDelegateHandle OnStartSessionDelegateHandle;
};
