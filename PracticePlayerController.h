#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PracticePlayerController.generated.h"

class UInputMappingContext;
class UInputAction;
class UUserWidget;

UCLASS()
class UEPRACTICE_API APracticePlayerController : public APlayerController
{
    GENERATED_BODY()

public:
    APracticePlayerController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputMappingContext* InputMappingContext;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* MoveAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* JumpAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* LookAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
    TSubclassOf<UUserWidget> HUDWidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
    UUserWidget* HUDWidgetInstance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
    TSubclassOf<UUserWidget> MainMenuWidgetClass;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
    UUserWidget* MainMenuWidgetInstance;

    virtual void BeginPlay() override;

    UFUNCTION(BlueprintPure, Category = "HUD")
    UUserWidget* GetHUDWidget() const;
    UFUNCTION(BlueprintCallable, Category = "HUD")
    void ShowGameHUD();
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void ShowMainMenu(bool bIsRestart);
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void StartGame();
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void QuitGame();
};