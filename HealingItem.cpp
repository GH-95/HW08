#include "HealingItem.h"
#include "PracticeCharacter.h"

AHealingItem::AHealingItem()
{
    HealAmount = 20.0f;
    ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
    Super::ActivateItem(Activator);

    if (Activator && Activator->ActorHasTag("Player"))
    {
        if (APracticeCharacter* PlayerCharacter = Cast<APracticeCharacter>(Activator))
        {
            PlayerCharacter->AddHealth(HealAmount);
        }
        DestroyItem();
    }
}