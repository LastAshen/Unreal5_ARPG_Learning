// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AuraAbilitySystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FEffectAssetTags, const FGameplayTagContainer&);
DECLARE_MULTICAST_DELEGATE_OneParam(FAbilitiesGiven,class UAuraAbilitySystemComponent*);
DECLARE_DELEGATE_OneParam(FForEachAbility,const FGameplayAbilitySpec&);

UCLASS()
class MYAURALEARNING_API UAuraAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	FEffectAssetTags EffectAssetTags;
	FAbilitiesGiven AbilitiesGivenDelegate;
	bool bStartupAbilitiesGiven;
	
	void AbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UGameplayAbility>> AbilityClasses);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>> PassiveAbilityClasses);
	
	void AbilityInputTagHeld(const FGameplayTag& Tag);
	void AbilityInputTagReleased(const FGameplayTag& Tag);
	void ForEachAbility(const FForEachAbility& Delegate);

	
protected:
	
	UFUNCTION(Client, Reliable)  //用以解决不在客户端执行的问题
	void ClientEffectApplied(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle EffectHandle);
};
