// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"

#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UAttributeMenuWidgetController::BindCallbacksToDependecies()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AS);

	for(auto& Pair : AS->TagsToAttributes)
	{
		//AbilitySystemComponent->RegisterAttributeChangeCallback(Pair.Key, AttributeInfoDelegate);  //TODO: ???

		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda([this, Pair](const FOnAttributeChangeData& Data)
		{
			BroadcastValues(Pair.Key, Pair.Value());
		});
	}
}

void UAttributeMenuWidgetController::BroadcastInitialValues()
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AS);

	for(auto& Pair : AS->TagsToAttributes)
	{
		BroadcastValues(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BroadcastValues(const FGameplayTag& Tag, const FGameplayAttribute& Attribute) const
{
	UAuraAttributeSet* AS = CastChecked<UAuraAttributeSet>(AttributeSet);
	check(AS);
	
	FAuraAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Tag);
	Info.AttributeValue = Attribute.GetNumericValue(AS);
	AttributeInfoDelegate.Broadcast(Info);
}
