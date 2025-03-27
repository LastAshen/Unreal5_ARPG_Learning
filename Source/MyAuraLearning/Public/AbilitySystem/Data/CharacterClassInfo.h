// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "Engine/DataAsset.h"
#include "CharacterClassInfo.generated.h"

UENUM(BlueprintType)
enum class ECharacterClass : uint8
{
	Elementalist,
	Warrior,
	Ranger
};

USTRUCT(BlueprintType)
struct FCharacterClassDefaultInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> PrimaryAttributeClass;
};

UCLASS()
class MYAURALEARNING_API UCharacterClassInfo : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category="Character Class Defaults");
	TMap<ECharacterClass, FCharacterClassDefaultInfo> CharacterClassInformation;
	
	UPROPERTY(EditAnywhere, Category="Common Class Defaults" )
	TSubclassOf<UGameplayEffect> SecondaryAttributes;
	
	UPROPERTY(EditAnywhere, Category="Common Class Defaults")
	TSubclassOf<UGameplayEffect> VitalAttributes;

	UPROPERTY(EditAnywhere, Category="Common Class Defaults")
	TArray<TSubclassOf<UGameplayAbility>> CommonAbilities;

	UPROPERTY(EditDefaultsOnly, Category="Common Class Defaults")
	TObjectPtr<UCurveTable> DamageCalcCoefCurveTable;

	//UFUNCTION(BlueprintCallable)
	FCharacterClassDefaultInfo GetDefaultInfo(ECharacterClass Class) const;

	
};














