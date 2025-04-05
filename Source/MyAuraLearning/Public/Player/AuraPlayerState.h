// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameFramework/PlayerState.h"
#include "AuraPlayerState.generated.h"

class UAttributeSet;
class UAuraAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChangeSignature, int32, Value);


UCLASS()
class MYAURALEARNING_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	AAuraPlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const {return AttributeSet;}
	FORCEINLINE int32 GetPlayerLevel() const {return Level;}   //TODO 学习一下FORCEINLINE的用法
	FORCEINLINE int32 GetPlayerXP() const {return XP;} 

	void AddToXP(int32 Amount);
	void SetXP(int32 NewXP);
	void SetLevel(int32 NewLevel);
	void AddToLevel(int32 Amount);

	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangeSignature OnXPChange;
	
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStatChangeSignature OnLevelChange;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UAuraAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAuraAttributeSet> AttributeSet;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 1;
	
	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	UFUNCTION()
	void OnRep_XP(int32 OldXP);
};
































