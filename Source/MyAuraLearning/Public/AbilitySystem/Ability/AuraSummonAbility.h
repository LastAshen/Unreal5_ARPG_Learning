// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "AuraSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class MYAURALEARNING_API UAuraSummonAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Summon")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MinSpawnDistance = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float MaxSpawnDistance = 250.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Summon")
	float SpawnSpread = 90.f;

protected:

	UFUNCTION(BlueprintCallable,  Category = "Summon")
	TArray<FVector> GetSpawnLocations();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Summon")
	int32 NumMinions= 5;

	
};
















