// All rights reserved Daniil Grigoriev


#include "UBDAbilitySystemComponent.h"

void UUBDAbilitySystemComponent::ReceiveDamage(UUBDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
