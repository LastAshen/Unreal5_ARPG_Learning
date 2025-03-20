// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystem_BFL.h"

#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/AuraWidgetController.h"

UOverlayWidgetController* UAuraAbilitySystem_BFL::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	//TODO 这里代码逻辑应该再整理下
	if(APlayerController* PC = UGameplayStatics	:: GetPlayerController(WorldContextObject, 0))
	{
		if(AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			auto PlayerState = PC->GetPlayerState<AAuraPlayerState>();
			auto AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			auto AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PlayerState, AbilitySystemComponent, AttributeSet);
			return HUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}

UAttributeMenuWidgetController* UAuraAbilitySystem_BFL::GetAttributeMenuWidgetController(const UObject* WorldContextObject)
{
	//TODO 这里代码逻辑应该再整理下
	if(APlayerController* PC = UGameplayStatics	:: GetPlayerController(WorldContextObject, 0))
	{
		if(AAuraHUD* HUD = Cast<AAuraHUD>(PC->GetHUD()))
		{
			auto PlayerState = PC->GetPlayerState<AAuraPlayerState>();
			auto AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			auto AttributeSet = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PlayerState, AbilitySystemComponent, AttributeSet);
			return HUD->GetAttributeMenuWidgetController(WidgetControllerParams);
		}
	}
	
	return nullptr;
}
