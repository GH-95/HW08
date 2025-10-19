#include "PracticeGameMode.h"
#include "PracticeCharacter.h"
#include "PracticePlayerController.h"
#include "PracticeGameState.h"

APracticeGameMode::APracticeGameMode()
{
	DefaultPawnClass = APracticeCharacter::StaticClass();
	PlayerControllerClass = APracticePlayerController::StaticClass();
	GameStateClass = APracticeGameState::StaticClass();
}