// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/AuraAbilitySystem_BFL.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

struct AuraDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	
	AuraDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
	}
	
};

static const AuraDamageStatics DamageStatics()
{
	static AuraDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef); 
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef); 
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef); 
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef); 
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef); 
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef); 
}

//TODO:  看不懂的代码
void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	auto SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	auto TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluationParams;
	EvaluationParams.SourceTags = SourceTags;
	EvaluationParams.TargetTags = TargetTags;
	
	float TargetArmor = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParams, TargetArmor);
	TargetArmor = FMath::Max(0.0f, TargetArmor);

	//Capture CriticalHitChance from Source
	float SourceCriticalHitChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParams, SourceCriticalHitChance);
	SourceCriticalHitChance = FMath::Max(SourceCriticalHitChance, 0.0f);
	
	//Capture CriticalHitDamage from Source
	float SourceCriticalHitDamage = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParams, SourceCriticalHitDamage);
	SourceCriticalHitDamage = FMath::Max(SourceCriticalHitDamage, 0.0f);
	
	//Capture CriticalHitResistance from Target
	float TargetCriticalHitResistance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParams, TargetCriticalHitResistance);
	TargetCriticalHitResistance = FMath::Max(TargetCriticalHitResistance, 0.0f);
	
	//Get Damage Set by Caller Magnitude
	float Damage = 0.f;
	for(auto Tag : FAuraGameplayTags::Get().DamageTypes)
	{
		Damage += Spec.GetSetByCallerMagnitude(Tag, false);
	}

	//Capture BlockChance from Target
	float TargetBlockChance = 0.0f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParams, TargetBlockChance);
	TargetBlockChance = FMath::Max(TargetBlockChance, 0.0f);
	
	//Capture ArmorPenetration from Source
	float SourceArmorPenetration = 0.0f;	
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParams, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max(SourceArmorPenetration, 0.0f);
	
	//Get Coefficients from Curve
	auto CharacterClassInfo = UAuraAbilitySystem_BFL:: GetCharacterClassInfo(SourceAvatar);
	auto ArmorPenetrationCoefCurve = CharacterClassInfo->DamageCalcCoefCurveTable->FindCurve(FName("ArmorPenetration"), FString());
	auto ArmorPenetrationCoef = ArmorPenetrationCoefCurve->Eval(SourceCombatInterface->GetPlayerLevel());

	auto EffectiveArmorCoefCurve = CharacterClassInfo->DamageCalcCoefCurveTable->FindCurve(FName("EffectiveArmor"), FString());
	auto EffectiveArmorCoef = EffectiveArmorCoefCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	//Calculate Damage
	float EffectiveCriticalHitChance = 50;// SourceCriticalHitChance - TargetCriticalHitResistance * 0.3f; 
	const bool bCriticalHit = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;
	Damage = bCriticalHit ? Damage *(2.f + SourceCriticalHitDamage / 100.f) : Damage;

	TargetBlockChance = 50.f;
	const bool bIsBlockedHit = FMath::RandRange(1, 100) < TargetBlockChance;
	Damage = bIsBlockedHit? Damage / 2 : Damage;

	auto EffectContextHandle = Spec.GetContext();
	UAuraAbilitySystem_BFL::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
	UAuraAbilitySystem_BFL::SetIsBlockedHit(EffectContextHandle, bIsBlockedHit);
	
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoef) / 100.f;
	Damage *=  (100 - EffectiveArmor * EffectiveArmorCoef) / 100.f;

	FGameplayModifierEvaluatedData EvaluationData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluationData);
}


















