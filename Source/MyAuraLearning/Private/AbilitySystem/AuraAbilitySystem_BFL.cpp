// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystem_BFL.h"

#include "AuraAbilityTypes.h"
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
	auto CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (!CharacterClassInfo)
		return;
	
	auto DefaultClassInfo = CharacterClassInfo->GetDefaultInfo(CharacterClass);
	auto EffectContextHandle = AbilitySystemComponent->MakeEffectContext();
	EffectContextHandle.AddSourceObject(AbilitySystemComponent->GetAvatarActor());
	
	auto PrimaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultClassInfo.PrimaryAttributeClass, Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

	auto SecondaryAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes,Level, EffectContextHandle);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

	auto VitalAttributesSpecHandle = AbilitySystemComponent->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes,Level, EffectContextHandle);
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

UCharacterClassInfo* UAuraAbilitySystem_BFL::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	auto AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));   //如果是客户端，这里gamemode应该是空的
	if (!AuraGameMode)
		 return nullptr;

	return AuraGameMode->CharacterClassInfo;
}

bool UAuraAbilitySystem_BFL::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsBlockedHit();
	}
	return false;
}

void UAuraAbilitySystem_BFL::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsBlockedHit)
{
	if( auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

bool UAuraAbilitySystem_BFL::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if(const auto AuraEffectContext = static_cast<const FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return AuraEffectContext->IsCriticalHit();
	}
	return false;
}

void UAuraAbilitySystem_BFL::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle,
	bool bInIsCriticalHit)
{
	if(auto AuraEffectContext = static_cast<FAuraGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		AuraEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}




























