// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"

#include "MyAuraLearning/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FAuraAttributeInfo& AttributeInfo : AttributeInformation)
	{
		if(AttributeInfo.AttributeTag .MatchesTagExact(AttributeTag))
		{
			return AttributeInfo;
		}
	}
	
	if(bLogNotFound)
	{
		UE_LOG(LogAura, Error, TEXT("AttributeInfo [%s] not found for tag %s"),*GetNameSafe(this), *AttributeTag.ToString());
	}
	
	return FAuraAttributeInfo();
}
