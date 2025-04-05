// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraDamageGameplayAbility.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Actor/AuraProjectile.h"
#include "AuraProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class MYAURALEARNING_API UAuraProjectileSpell : public UAuraDamageGameplayAbility
{
	GENERATED_BODY()

	
	
protected: //Functions
	virtual  void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable)
	void SpawnProjectile(const FVector& TargetLocation,bool OverridePitch,float Pitch, AActor* HomingTarget);
	
protected://Properties
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAuraProjectile> ProjectileClass;
	
};
