// Fill out your copyright notice in the Description page of Project Settings.


#include "UBDAbilitySystemComponent.h"

void UUBDAbilitySystemComponent::ReceiveDamage(UUBDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}
