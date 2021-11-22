// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "GameFramework/GameModeBase.h"
#include "assignmentGameMode.generated.h"

class AassignmentCharacter;

UCLASS(minimalapi)
class AassignmentGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	AassignmentGameMode();

	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void PauseGame();

	UFUNCTION(BlueprintCallable)
	void ResumeGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();
	
	UFUNCTION(BlueprintPure)
	bool HasGameEnded();

	UFUNCTION(BlueprintCallable)
	void SetInteractableInfo(FString Name, FString Description, FString InteractDescription);

	UFUNCTION(BlueprintCallable)
	void SetInteractNotification(FString InteractNotification);

	UFUNCTION(BlueprintPure)
	FString GetInteractableInfo();

	UFUNCTION(BlueprintPure)
	FString GetInteractNotification();

	UFUNCTION(BlueprintPure)
	FString GetGameEndMessage();

	UFUNCTION(BlueprintPure)
	float GetPlayerHealthPercentage();

	UFUNCTION(BlueprintCallable)
	void SetHasWon(bool Won);
	
	UFUNCTION(BlueprintPure)
	TArray<class UTexture2D*> GetPlayerItems();
protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CheckPlayerHealth(float Health);

	UFUNCTION(BlueprintCallable)
	void ClearInteractNotification();

	// Interactable information to display.
	FString CurrentInteractableName;
	FString CurrentInteractableDescription;
	FString CurrentInteractDescription;
	FString CurrentInteractNotification;
	FTimerHandle InteractNotificationTimer;

	AassignmentCharacter* Player;
	bool IsPlaying;
	bool IsPaused;
	bool IsPlayerDead;
	bool HasWon;

	class UTexture2D* ItemFrameTexture;
	class UTexture2D* KeyTexture;
	class UTexture2D* BatteryTexture;
	class UTexture2D* FuseTexture;
	class UTexture2D* StunGrenadeTexture;

	ULevelSequence* StartSequence;
};