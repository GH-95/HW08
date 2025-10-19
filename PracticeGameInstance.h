#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PracticeGameInstance.generated.h"

UCLASS()
class UEPRACTICE_API UPracticeGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPracticeGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};