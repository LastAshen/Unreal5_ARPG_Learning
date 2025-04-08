// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Ability/AuraGameplayAbility.h"
#include "Blink.generated.h"

/**
 * 
 */
UCLASS()
class MYAURALEARNING_API UBlink : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	FVector GetBlinkLocation();

	UPROPERTY(EditDefaultsOnly)
	float BlinkDistance = 300.f;

	UPROPERTY(EditDefaultsOnly)
	FVector TargetLocationOffset = FVector(0.f, 0.f, 60.f);
};
