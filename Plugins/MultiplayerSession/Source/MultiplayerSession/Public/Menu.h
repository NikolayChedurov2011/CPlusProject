// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	void ShowMenu();

protected:

	//virtual bool Initialize() override;
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;

	void OnSessionUpdated(const TArray<FOnlineSessionSearchResult>& SessionSearchResult);

private:

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Create;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Refresh;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Join;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ScrlBox_ServerList;
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TxtBox_IP;
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* CheckBox_IsLan; 
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* TxtBox_SessionName;
	
	UFUNCTION()
	void OnCreateClicked();
	UFUNCTION()
	void OnRefreshClicked();

	void MenuTearDown();

	class UOnlineSubsystemPlugin* OnlineSubsystemPlugin;
};
