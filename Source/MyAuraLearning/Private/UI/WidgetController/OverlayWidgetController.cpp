// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "MyAuraLearning/AuraLogChannels.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AuraAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AuraAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependecies()
{
	auto AuraAttributeSet = CastChecked<UAuraAttributeSet>(AttributeSet);
	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnMaxHealthChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data)
		{
			OnManaChanged.Broadcast(Data.NewValue);
		});
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		AuraAttributeSet->GetMaxManaAttribute()).AddLambda([this](	const FOnAttributeChangeData& Data)
		{
			OnMaxManaChanged.Broadcast(Data.NewValue);
		});

	if(auto ASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		if(ASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(ASC);
		}
		else
		{
			ASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);
		}
	
		ASC-> EffectAssetTags.AddLambda([this](const FGameplayTagContainer& Container)
		{
			for (const FGameplayTag& Tag : Container)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (Tag.MatchesTag(MessageTag))
				{
					if (const auto Row = GetDataTableRowByTag<FUIWidgetRow>(MessageDataTable, Tag))
					{
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		});
	}
}

void UOverlayWidgetController::OnInitializeStartupAbilities(UAuraAbilitySystemComponent* ASC)
{
	if(!ASC->bStartupAbilitiesGiven) return;
	
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, ASC](const FGameplayAbilitySpec& Spec)
	{
		FGameplayTag AbilityTag;
		for (const FGameplayTag& Tag : Spec.Ability.Get()->AbilityTags)
		{
			if (Tag.IsValid() && Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("Ability"))))
				AbilityTag = Tag;
		}

		if (!AbilityTag.IsValid())
		{
			UE_LOG(LogAura,Error, TEXT("No Ability Tag found for Ability %s"), *Spec.Ability.Get()->GetName())
			return;
		}
		
		FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoByTag(AbilityTag, false);
		for(const FGameplayTag& Tag : Spec.DynamicAbilityTags)
		{
			if(Tag.IsValid() && Tag.MatchesTag(FGameplayTag::RequestGameplayTag(FName("InputTag"))))
				Info.InputTag = Tag;
		}
		AbilityInfoDelegate.Broadcast(Info);
	});

	ASC->ForEachAbility(BroadcastDelegate);
}



























