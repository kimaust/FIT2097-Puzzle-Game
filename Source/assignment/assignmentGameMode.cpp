// Copyright Epic Games, Inc. All Rights Reserved.

#include "assignmentGameMode.h"

#include "assignmentHUD.h"
#include "assignmentCharacter.h"
#include "Internationalization/Text.h"
#include "UObject/ConstructorHelpers.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "assignmentCharacter.h"
#include "AIEnemyCharacter.h"
#include "AIEnemyController.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

AassignmentGameMode::AassignmentGameMode()
	: Super()
{
	PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FObjectFinder<ULevelSequence> StartSequenceObj(TEXT("/Game/Cinematics/Master.Master"));
	StartSequence = StartSequenceObj.Object;
	
	// use our custom HUD class
	// HUDClass = AassignmentHUD::StaticClass();

	// Load-up some textures for items.
	static ConstructorHelpers::FObjectFinder<UTexture2D> ItemFrameObj(TEXT("/Game/Texture/ItemFrame.ItemFrame"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> KeyObj(TEXT("/Game/Texture/KeyTexture.KeyTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> BatteryObj(TEXT("/Game/Texture/BatteryTexture.BatteryTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> FuseObj(TEXT("/Game/Texture/FuseTexture.FuseTexture"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> StunGrenadeObj(TEXT("/Game/Texture/StunGrenadeTexture.StunGrenadeTexture"));
	ItemFrameTexture = ItemFrameObj.Object;
	KeyTexture = KeyObj.Object;
	BatteryTexture = BatteryObj.Object;
	FuseTexture = FuseObj.Object;
	StunGrenadeTexture = StunGrenadeObj.Object;

	IsPlaying = false;
	IsPlayerDead = false;
	IsPaused = false;
	HasWon = false;
}

void AassignmentGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Set up the event listener for the player health to detect when health becomes 0.
	Player = Cast<AassignmentCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Player->HealthEventDispatcher.AddDynamic(this, &AassignmentGameMode::CheckPlayerHealth);
	IsPlaying = true;

	// Play sequence cinematics.
	ALevelSequenceActor* SequenceActor;
	FMovieSceneSequencePlaybackSettings PlaybackSettings;
	ULevelSequencePlayer* SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(), StartSequence, PlaybackSettings, SequenceActor);
	if (SequencePlayer) {
		SequencePlayer->Play();
	}
}

void AassignmentGameMode::StartGame()
{
	ResumeGame();
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Level/MainLevel");

	IsPlaying = true;
	IsPlayerDead = false;
	IsPaused = false;
	HasWon = false;
}

void AassignmentGameMode::PauseGame()
{
	IsPaused = true;
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void AassignmentGameMode::ResumeGame()
{
	IsPaused = false;
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void AassignmentGameMode::QuitGame()
{
	IsPlaying = false;
	UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}

bool AassignmentGameMode::HasGameEnded()
{
	return IsPlayerDead || HasWon;
}

void AassignmentGameMode::SetInteractableInfo(FString Name, FString Description, FString InteractDescription)
{
	CurrentInteractableName = Name;
	CurrentInteractableDescription = Description;
	CurrentInteractDescription = InteractDescription;
}

void AassignmentGameMode::SetInteractNotification(FString InteractNotification)
{
	CurrentInteractNotification = InteractNotification;

	// Set timer to clear the notification after 5 seconds.
	GetWorldTimerManager().SetTimer(InteractNotificationTimer, this, &AassignmentGameMode::ClearInteractNotification, 5, true);
}

FString AassignmentGameMode::GetInteractableInfo()
{
	FString InteractableInfo;
	
	InteractableInfo += CurrentInteractableName + LINE_TERMINATOR;
	InteractableInfo += CurrentInteractableDescription + LINE_TERMINATOR;
	InteractableInfo += CurrentInteractDescription + LINE_TERMINATOR;

	return InteractableInfo;
}

FString AassignmentGameMode::GetInteractNotification()
{
	return CurrentInteractNotification;
}

FString AassignmentGameMode::GetGameEndMessage()
{
	FString EndMessage;
	if (HasWon) {
		EndMessage = "You've escaped the room! You Win!";
	} else if (IsPlayerDead) {
		EndMessage = "You've died. You Lose!";
	}
	return EndMessage;
}

float AassignmentGameMode::GetPlayerHealthPercentage()
{
	return Player->Health / Player->MaxHealth;
}

void AassignmentGameMode::SetHasWon(bool Won)
{
	HasWon = Won;
}

TArray<UTexture2D*> AassignmentGameMode::GetPlayerItems()
{
	TArray<UTexture2D*> ItemTextures;

	// Map the items to its corresponding textures.
	for (auto& Item : Player->Items) {
		if (Item == "Key") {
			ItemTextures.Add(KeyTexture);
		} else if (Item == "Fuse") {
			ItemTextures.Add(FuseTexture);
		} else if (Item == "StunGrenade") {
			ItemTextures.Add(StunGrenadeTexture);
		}
	}
	// Fill the rest of the array with the default item frame texture for empty items.
	for (int i = ItemTextures.Num(); i < 4; ++i) {
		ItemTextures.Add(ItemFrameTexture);
	}
	return ItemTextures;
}

void AassignmentGameMode::CheckPlayerHealth(float Health)
{
	if (Health <= 0) {
		IsPlayerDead = true;
	}
}

void AassignmentGameMode::ClearInteractNotification()
{
	GetWorldTimerManager().ClearTimer(InteractNotificationTimer);
	CurrentInteractNotification = FString(TEXT(""));
}