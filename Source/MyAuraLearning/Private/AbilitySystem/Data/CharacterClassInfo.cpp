// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetDefaultInfo(ECharacterClass Class) const
{
	return *CharacterClassInformation.Find(Class);
}
