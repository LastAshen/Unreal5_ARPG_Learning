// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AuraAbilitySystemComponent.h"

#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "MyAuraLearning/AuraLogChannels.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::ClientEffectApplied);
}

void UAuraAbilitySystemComponent::AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> AbilityClasses)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : AbilityClasses)
	{
		 FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		
		if(const UAuraGameplayAbility* AuraAbility = Cast<UAuraGameplayAbility>(AbilitySpec.Ability))
		{
			AbilitySpec.DynamicAbilityTags.AddTag(AuraAbility->StartupInputTag);
			GiveAbility(AbilitySpec);
		}
	}

	bStartupAbilitiesGiven = true;
	AbilitiesGivenDelegate.Broadcast(this);
}

void UAuraAbilitySystemComponent::AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> PassiveAbilityClasses)
{
	for (TSubclassOf<UGameplayAbility> AbilityClass : PassiveAbilityClasses)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		GiveAbilityAndActivateOnce(AbilitySpec);
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagHeld(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;
	for(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(Spec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputPressed(Spec);
			if(!Spec.IsActive())
			{
				TryActivateAbility(Spec.Handle);
			}
		}
	}
}

void UAuraAbilitySystemComponent::AbilityInputTagReleased(const FGameplayTag& Tag)
{
	if(!Tag.IsValid()) return;
	for(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{  
		if(Spec.DynamicAbilityTags.HasTagExact(Tag))
		{
			AbilitySpecInputReleased(Spec);
		}
	}
}

void UAuraAbilitySystemComponent::ForEachAbility(const FForEachAbility& Delegate)
{
	FScopedAbilityListLock ActiveScopeLock(*this);
	for(FGameplayAbilitySpec& Spec : GetActivatableAbilities())
	{
		if(!Delegate.ExecuteIfBound(Spec))
		{
			UE_LOG(LogAura, Error, TEXT("Failed to execute delegate for ability in %hs"), __FUNCTION__);
		}
	}
}

void UAuraAbilitySystemComponent::ClientEffectApplied_Implementation(UAbilitySystemComponent* AbilitySystemComponent,
                                                                     const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}











