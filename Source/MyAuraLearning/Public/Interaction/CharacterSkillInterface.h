// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CharacterSkillInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UCharacterSkillInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYAURALEARNING_API ICharacterSkillInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Skill Interface")
	void ChargeJumping();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Skill Interface")
	void OnBeginElectricLoadCast();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Skill Interface")
	void OnElectricLoadCastLanding();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Character Skill Interface")
	void OnBeginHurricaneKickMove();
	
};
