// Fill out your copyright notice in the Description page of Project Settings.


#include "UBDAssetManger.h"
#include "AbilitySystemGlobals.h"

void UUBDAssetManger::StartInitialLoading()
{
	Super::StartInitialLoading();
	UAbilitySystemGlobals::Get().InitGlobalData();
}
