// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerState.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.0f;
	// SetNetUpdateFrequency(100.0f); //This is for version 5.5 and above

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>(TEXT("AttributeSet"));
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToXP(int32 Amount)
{
	XP += Amount;
	OnXPChange.Broadcast(XP);
}

void AAuraPlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChange.Broadcast(XP);
}

void AAuraPlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChange.Broadcast(Level);
}

void AAuraPlayerState::AddToLevel(int32 Amount)
{
	Level += Amount;
	OnLevelChange.Broadcast(Level);
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	//GAMEPLAYATTRIBUTE_REPNOTIFY(AAuraPlayerState, Level, OldLevel);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
}


