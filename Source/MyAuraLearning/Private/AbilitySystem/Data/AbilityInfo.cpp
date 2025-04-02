// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AbilityInfo.h"

#include "MyAuraLearning/AuraLogChannels.h"

FAuraAbilityInfo UAbilityInfo::FindAbilityInfoByTag(FGameplayTag AbilityTag, bool bLogNotFound) const
{
	for (const auto& AbilityInfo : AbilityInformation)
	{
		if(AbilityInfo.AbilityTag == AbilityTag)
		{
			return AbilityInfo;
		}
	}
	
	if(bLogNotFound)
	{
		UE_LOG(LogAura, Warning, TEXT("AbilityInfo not found for tag: [%s]"), *AbilityTag.ToString());
	}
	return FAuraAbilityInfo();
}
