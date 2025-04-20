// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/CharacterSkillInterface.h"
#include "AuraCharacter.generated.h"

class USkillSystemComponent;
/**
 * 
 */
UCLASS()
class MYAURALEARNING_API AAuraCharacter : public AAuraCharacterBase, public ICharacterSkillInterface
{
	GENERATED_BODY()
	
public:
	AAuraCharacter();
	//AAuraCharacter(const FObjectInitializer& ObjectInitializer);

	//ICombatInterface
	virtual int32 GetPlayerLevel() override;
	//end ICombatInterface

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
	
	virtual void InitAbilityActorInfo() override;

	virtual void Die() override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Skill System")
	TObjectPtr<USkillSystemComponent> SkillSystemComp;
};
































