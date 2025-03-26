// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Data/CharacterClassInfo.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AuraAbilitySystem_BFL.generated.h"

class UAttributeMenuWidgetController;
class UOverlayWidgetController;


UCLASS()
class MYAURALEARNING_API UAuraAbilitySystem_BFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library | Widget Controller")
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Widget Controller")
	static UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Character Class Defaults")
	static void InitializeDefaultAttributesForCharacterClass(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* AbilitySystemComponent);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Character Class Defaults")
	static  void GiveStartupAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent);
};
