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
	static  void GiveStartupAttributes(const UObject* WorldContextObject, UAbilitySystemComponent* AbilitySystemComponent, ECharacterClass CharacterClass);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Character Class Defaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library | Gameplay Effects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Gameplay Effects")
	static void SetIsBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintPure, Category = "Aura Ability System Library | Gameplay Effects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Gameplay Effects")
	static void SetIsCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Gameplay Mechanics")
	static void GetLivePlayersWithinRadius(const UObject* WorldContextObject,const FVector& Origin, float Radius,UPARAM(ref) TArray<AActor*>& ActorsToIgnore, TArray<AActor*>& OutActors);
	
	UFUNCTION(BlueprintCallable, Category = "Aura Ability System Library | Gameplay Mechanics")
	static bool IsFriends(const AActor* Actor1, const AActor* Actor2);

	UFUNCTION(BlueprintCallable, Category = "Aura Controller Helper")
	static FHitResult GetPlayerCursorHitResult(const UObject* WorldContextObject);


	// Helper function to find a value in a map by key and return true if found, false otherwise
	template<typename K, typename V>
	static bool FindByCheckIfContains(const TMap<K,V>& Map, K Key, V& OutValue)
	{
		if(Map.Contains(Key))
		{
			OutValue = *Map.Find(Key);
			return true;
		}
		else
		{
			return false;
		}
	}
};






















