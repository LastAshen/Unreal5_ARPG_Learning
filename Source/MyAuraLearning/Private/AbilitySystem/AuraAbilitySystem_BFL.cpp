// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystem_BFL.h"

#include "AuraAbilityTypes.h"
#include "Engine/OverlapResult.h"
#include "Game/AuraGameModeBase.h"
#include "Interaction/CombatInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerController.h"
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

void UAuraAbilitySystem_BFL::GiveStartupAttributes(const UObject* WorldContextObject,UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass)
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

	for(auto AbilityClass: AuraGameMode->CharacterClassInfo->GetDefaultInfo(CharacterClass).StartupAbilities)
	{
		  if(auto CombatInterface = Cast<ICombatInterface>(AbilitySystemComponent->GetAvatarActor()))
		  {
				auto AbilitiSpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());
				AbilitySystemComponent->GiveAbility(AbilitiSpec);
		  }
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

void UAuraAbilitySystem_BFL::GetLivePlayersWithinRadius(const UObject* WorldContextObject, const FVector& Origin,
	float Radius, TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorsToIgnore);
	if(UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		TArray<FOverlapResult> Overlaps;
		World->OverlapMultiByObjectType(Overlaps, Origin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radius), SphereParams);
		for(auto& Overlap : Overlaps)
		{
			 //注意这里应该使用 UCombatInterface   ↓↓↓   而非 ICombatInterface
			if(Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

bool UAuraAbilitySystem_BFL::IsFriends(const AActor* Actor1, const AActor* Actor2)
{
	if(Actor1 == nullptr || Actor2 == nullptr)
		return false;
	return Actor1->ActorHasTag("Player") && Actor2->ActorHasTag("Player") || Actor1->ActorHasTag("Enemy") && Actor2->ActorHasTag("Enemy");
}

FHitResult UAuraAbilitySystem_BFL::GetPlayerCursorHitResult(const UObject* WorldContextObject)
{
	if(auto World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		auto Controller = UGameplayStatics::GetPlayerController(World, 0);
		if(auto AuraController = Cast<AAuraPlayerController>(Controller))
		{
			return AuraController->GetCursorHitResult();
		}
	}

	return FHitResult();
}




























