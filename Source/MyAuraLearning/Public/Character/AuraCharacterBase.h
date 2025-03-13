// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystemInterface.h"
#include "Interaction/CombatInterface.h"
#include "AuraCharacterBase.generated.h"

class UAttributeSet;
class UAuraAttributeSet;
class UAuraAbilitySystemComponent;

UCLASS(abstract)
class MYAURALEARNING_API AAuraCharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AAuraCharacterBase();
	//AAuraCharacterBase(const FObjectInitializer& ObjectInitializer);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAttributeSet> AttributeSet;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultPrimaryAttributeClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultSecondaryAttributeClass;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "Attribute")
	TSubclassOf<UGameplayEffect> DefaultVitalAttributeClass;

	virtual void InitAbilityActorInfo();
	void ApplyEffectToSelf(TSubclassOf<UGameplayEffect> AttributeClass, float Level = 1.0f) const;
	void InitializeDefaultAttributes()	const;
};
