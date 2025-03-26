// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystem_BFL.h"

#include "Game/AuraGameModeBase.h"
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

void UAuraAbilitySystem_BFL::InitializeDefaultAttributesForCharacterClass(const UObject* WorldContextObject,
	ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent)
{
	auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));   //如果是客户端，这里gamemode应该是空的
	if (!AuraGameMode)
		return;
	
	auto DefaultClassInfo = AuraGameMode->CharacterClassInfo->GetDefaultInfo(CharacterClass);
	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	
	auto PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultClassInfo.PrimaryAttributeClass, Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	auto SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->SecondaryAttributes,Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	auto VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(AuraGameMode->CharacterClassInfo->VitalAttributes,Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());
	
}

void UAuraAbilitySystem_BFL::GiveStartupAttributes(const UObject* WorldContextObject,UAbilitySystemComponent* AbilitySystemComponent)
{
	auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));   //如果是客户端，这里gamemode应该是空的
	if (!AuraGameMode)
		return;

	auto ClassInfo = AuraGameMode->CharacterClassInfo;
	for(auto AbilityClass: ClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		AbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}




























