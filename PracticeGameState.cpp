#include "PracticeGameState.h"
#include "PracticeGameInstance.h"
#include "PracticePlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

APracticeGameState::APracticeGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 20.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
	ItemsToSpawnPerWave = { 20, 30, 40 };
}

void APracticeGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&APracticeGameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 APracticeGameState::GetScore() const
{
	return Score;
}

void APracticeGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UPracticeGameInstance* PracticeGameInstance = Cast<UPracticeGameInstance>(GameInstance);
		if (PracticeGameInstance)
		{
			PracticeGameInstance->AddToScore(Amount);
		}
	}
}

void APracticeGameState::StartLevel()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APracticePlayerController* PracticePlayerController = Cast<APracticePlayerController>(PlayerController))
		{
			PracticePlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UPracticeGameInstance* PracticeGameInstance = Cast<UPracticeGameInstance>(GameInstance);
		if (PracticeGameInstance)
		{
			CurrentLevelIndex = PracticeGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	StartWave();
	/*CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	if (FoundVolumes.Num() > 0)
	{
		ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
		if (SpawnVolume)
		{
			for (int32 i = 0; i < ItemToSpawn; i++)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}*/
}

void APracticeGameState::OnLevelTimeUp()
{
	EndWave();
}

void APracticeGameState::OnCoinCollected()
{
	CollectedCoinCount++;

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void APracticeGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UPracticeGameInstance* PracticeGameInstance = Cast<UPracticeGameInstance>(GameInstance);
		if (PracticeGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			PracticeGameInstance->CurrentLevelIndex = CurrentLevelIndex;

			if (CurrentLevelIndex >= MaxLevels)
			{
				OnGameOver();
				return;
			}

			if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
			{
				UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
			}
			else
			{
				OnGameOver();
			}
		}
	}
}

void APracticeGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APracticePlayerController* PracticePlayerController = Cast<APracticePlayerController>(PlayerController))
		{
			PracticePlayerController->SetPause(true);
			PracticePlayerController->ShowMainMenu(true);
		}
	}
}

void APracticeGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(APracticePlayerController* PracticePlayerController = Cast<APracticePlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = PracticePlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UPracticeGameInstance* PracticeGameInstance = Cast<UPracticeGameInstance>(GameInstance);
						if (PracticeGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), PracticeGameInstance->TotalScore)));
						}
					}
				}

				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
				}

				if (UTextBlock* WaveText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d / %d"), CurrentWaveIndex + 1, MaxWaves)));
				}
			}
		}
	}
}

void APracticeGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	for (AActor* Item : CurrentWaveItems)
	{
		if (Item && Item->IsValidLowLevelFast())
		{
			Item->Destroy();
		}
	}

	CurrentWaveItems.Empty();

	int32 ItemToSpawn = (ItemsToSpawnPerWave.IsValidIndex(CurrentWaveIndex)) ? ItemsToSpawnPerWave[CurrentWaveIndex] : 20;

	if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
	{
		for (int32 i = 0; i < ItemToSpawn; i++)
		{
			if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem())
			{
				if (SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}

				CurrentWaveItems.Add(SpawnedActor);
			}
		}
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APracticePlayerController* PracticePlayerController = Cast<APracticePlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = PracticePlayerController->GetHUDWidget())
			{
				UFunction* PlayAnimFunc = HUDWidget->FindFunction(FName("PlayShowWaveNotifyAnim"));

				if (PlayAnimFunc)
				{
					HUDWidget->ProcessEvent(PlayAnimFunc, nullptr);
				}

				if (UTextBlock* WaveNotifyText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName("WaveNotifyText")))
				{
					WaveNotifyText->SetText(FText::FromString(
						FString::Printf(TEXT("Wave %d ¹ß»ý!"), CurrentWaveIndex + 1)));
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&APracticeGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

ASpawnVolume* APracticeGameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return (FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}

APracticePlayerController* APracticeGameState::GetPracticePlayerController() const
{
	return Cast<APracticePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

void APracticeGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	++CurrentWaveIndex;
	if (CurrentWaveIndex >= MaxWaves)
	{
		EndLevel();
	}
	else
	{
		StartWave();
	}
}

