// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraAbilitySystemComponent.h"
#include "GameFramework/Actor.h"
#include "SkillSystemComponent.generated.h"

struct FGameplayTag;
class UGameplayAbility;

#define MAXWALKSPEEDONSKILL 200.f;
#define MAXWALKSPEEDNORMALLY 550.f;

DECLARE_MULTICAST_DELEGATE_OneParam(FSkillIntValueChangedDelegate, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FSkillFloatValueChangedDelegate, float);

UENUM()
enum class ESkillState : uint8
{
	None,
	NormalA,
	NormalB,
	NormalC,
	Blink,
	BlinkFollowup,
	Charge,
	PowerCast
};

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYAURALEARNING_API USkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:	
	USkillSystemComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void Initialize(UAbilitySystemComponent* InAbilitySystemComponent, ACharacter* InCharacter);

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void OnAbilityEnd(UGameplayAbility* InAbility);

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	bool InputAbility(FGameplayTag InputTag);

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	bool IfHaveMaxChargeCount(){ return ChargeCount >= MaxChargeCount; }

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void SetChargeCount(int32 NewChargeCount){ ChargeCount = FMath::Clamp(NewChargeCount, 0, MaxChargeCount); OnChargeCountChangedDelegate.Broadcast(ChargeCount); }

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void AddChargeCount(int32 NewAddValue){ ChargeCount = FMath::Clamp(ChargeCount + NewAddValue, 0, MaxChargeCount);; OnChargeCountChangedDelegate.Broadcast(ChargeCount); }

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void AddEnergyPointCount(int32 NewAddValue){ EnergyPointCount = FMath::Clamp(EnergyPointCount + NewAddValue, 0, MaxEnergyPointCount); OnEnergyPointCountChangedDelegate.Broadcast(EnergyPointCount); }
	
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void SetEnergyPointCount(int32 NewChargeCount){ EnergyPointCount =  FMath::Clamp(NewChargeCount, 0, MaxEnergyPointCount); OnEnergyPointCountChangedDelegate.Broadcast(EnergyPointCount); }
	
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	int32 GetMaxChargeCount(){return MaxChargeCount;}

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	int32 GetMaxEnergyPointCount(){return EnergyPointCount;}
	
	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	bool IfHaveMaxEnergyPointCount(){ return EnergyPointCount >= MaxEnergyPointCount; }

	UFUNCTION()
	void OnSkillPressed(FGameplayTag AbilityTag);

	UFUNCTION()
	void OnSkillReleased(FGameplayTag AbilityTag);

	UFUNCTION()
	void OnSkillHeld(FGameplayTag AbilityTag);

	FSkillIntValueChangedDelegate OnChargeCountChangedDelegate;
	FSkillIntValueChangedDelegate  OnEnergyPointCountChangedDelegate;
	
protected:

	//Cant use next skill immediately after previous skill ended
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	float SkillProtectTime = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	float NormalSkill_A_WaitTime = 1.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	float NormalSkill_B_WaitTime = 1.f;

	UPROPERTY(BlueprintReadOnly, Category = "SkillSystem")
	int32 ChargeCount = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	int32 MaxChargeCount = 12;
	
	UPROPERTY(BlueprintReadOnly, Category = "SkillSystem")
	int32 EnergyPointCount = 0;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	int32 MaxEnergyPointCount = 3;

	UPROPERTY(BlueprintReadOnly, Category = "SkillSystem")
	ESkillState CurrentSkillState = ESkillState::None;

	UPROPERTY(BlueprintReadOnly, Category = "SkillSystem")
	TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void StartSkillProtectTimer();

	UFUNCTION(BlueprintCallable, Category = "SkillSystem")
	void StartWaitFollowupTimer(float WaitFollowupTime);
	
	bool TryActiveSkill(ESkillState SkillState);
	
	//Abilities
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "SkillSystem")
	TArray<TSubclassOf<UGameplayAbility>> NormalAbility_ABC_Class;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	TSubclassOf<UGameplayAbility> BlinkAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	TSubclassOf<UGameplayAbility> BlinkFollowupAbilityClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	TSubclassOf<UGameplayAbility> ChargeAbilityClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SkillSystem")
	TSubclassOf<UGameplayAbility> PowerCastAbilityClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkillSystem")
	TObjectPtr<ACharacter> Character;
	
private:

	bool bInSkillProtectTime = false;
	bool bInWaitFollowupTime = false;
	FGameplayTag CurrentExecutingInputTag;
	FTimerHandle WaitFollowupTimer;
	FTimerDelegate WaitFollowupTimerDelegate;
	TMap<ESkillState, FGameplayAbilitySpecHandle> SkillStateToAbilitySpecHandleMap;

	void OnFollowupTimerEnd();
	FGameplayAbilitySpec GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass);
};























