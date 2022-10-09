// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSlot.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API USessionSlot : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetResult(const FOnlineSessionSearchResult& SessionSearchResult);

private:

protected:

	class UOnlineSubsystemPlugin* OnlineSubsystemPlugin;

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_JoinServer;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_ServerName;

	FOnlineSessionSearchResult SearchResult;

	UFUNCTION()
	void OnJoinButtonClicked();
};
