// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Actor/AuraProjectile.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class MYAURALEARNING_API UAuraProjectileSpell : public UAuraGameplayAbility
{
	GENERATED_BODY()

	
	
protected: //Functions
	virtual  void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& TargetLocation);
	
protected://Properties
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly);
	TSubclassOf<UGameplayEffect> DamageEffect;
};
