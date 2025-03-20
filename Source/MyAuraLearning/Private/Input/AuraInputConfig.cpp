// Fill out your copyright notice in the Description page of Project Settings.


#include "Input/AuraInputConfig.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& Tag, bool bLogNotFound)
{
	for (const auto& Action : AbilityInputActions)
	{
		if(Action.InputAction&& Action.InputTag == Tag)
			return Action.InputAction;
	}

	if(bLogNotFound)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unable to find input action for tag %s"), *Tag.ToString());
	}
	return nullptr;
}
