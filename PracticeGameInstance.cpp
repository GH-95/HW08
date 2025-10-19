#include "PracticeGameInstance.h"

UPracticeGameInstance::UPracticeGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UPracticeGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
	UE_LOG(LogTemp, Warning, TEXT("Total Score Updated: %d"), TotalScore);
}