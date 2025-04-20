// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/SkillSystemComponent.h"

#include "AuraGameplayTags.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyAuraLearning/AuraLogChannels.h"

USkillSystemComponent::USkillSystemComponent()
{
	//PrimaryActorTick.bCanEverTick = false;
}

void USkillSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	SetChargeCount(0);
	SetEnergyPointCount(0);
}

void USkillSystemComponent::Initialize(UAbilitySystemComponent* InAbilitySystemComponent, ACharacter* InCharacter)
{
	AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(InAbilitySystemComponent);
	Character = InCharacter;
	if(!AuraAbilitySystemComponent || !Character)
	{
		UE_LOG(LogAura, Error, TEXT("Initialize failed: Invalid AbilitySystemComponent or Character."))
		return;
	}

	if(NormalAbility_ABC_Class.Num() < 3)
	{
		UE_LOG(LogAura, Error, TEXT("Initialize failed: Normal Skills not set up correctly."))
		return;
	}
	
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::NormalA, GiveAbility(NormalAbility_ABC_Class[0]).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::NormalB, GiveAbility(NormalAbility_ABC_Class[1]).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::NormalC, GiveAbility(NormalAbility_ABC_Class[2]).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::Blink, GiveAbility(BlinkAbilityClass).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::BlinkFollowup, GiveAbility(BlinkFollowupAbilityClass).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::Charge, GiveAbility(ChargeAbilityClass).Handle);
	SkillStateToAbilitySpecHandleMap.Add(ESkillState::PowerCast, GiveAbility(PowerCastAbilityClass).Handle);

	AuraAbilitySystemComponent->AbilityEndedCallbacks.AddUObject(this, &USkillSystemComponent::OnAbilityEnd);
	WaitFollowupTimerDelegate.BindUObject(this, &USkillSystemComponent::OnFollowupTimerEnd);
}


void USkillSystemComponent::OnAbilityEnd(UGameplayAbility* InAbility)
{
	Character->GetCharacterMovement()->MaxWalkSpeed = MAXWALKSPEEDNORMALLY;
	if(CurrentSkillState == ESkillState::NormalA)
	{
		StartWaitFollowupTimer(NormalSkill_A_WaitTime);
	}
	else if (CurrentSkillState == ESkillState::NormalB)
	{
		StartWaitFollowupTimer(NormalSkill_B_WaitTime);
	}
	else
	{
		CurrentSkillState = ESkillState::None;
	}
}

bool USkillSystemComponent::InputAbility(FGameplayTag InputTag)
{
	//if(InputTag == FAuraGameplayTags::Get().InputTag_LMB)    // 这里==？？？？
	if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		switch (CurrentSkillState)
		{ 
			case ESkillState::NormalA:
				WaitFollowupTimerDelegate.Execute();
				GetWorld()->GetTimerManager().ClearTimer(WaitFollowupTimer);
				return TryActiveSkill(ESkillState::NormalB); 
			case ESkillState::NormalB:
				WaitFollowupTimerDelegate.Execute();
				GetWorld()->GetTimerManager().ClearTimer(WaitFollowupTimer);
				return TryActiveSkill(ESkillState::NormalC);
			default:
				return TryActiveSkill(ESkillState::NormalA);
		}
	}
	else if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_RMB))
	{
		return TryActiveSkill(ESkillState::Blink);	
	}
	else if(InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_SpaceBar))
	{
		//If have Max energy point, start power cast
		if(IfHaveMaxEnergyPointCount())
		{
			//
			if( TryActiveSkill(ESkillState::Charge))
			{
				SetChargeCount(0);
				return true;
			}
			return false;
		}
		//If have max charge count, start charge cast
		if(IfHaveMaxChargeCount())
		{
			//
			if(TryActiveSkill(ESkillState::PowerCast))
			{
				SetEnergyPointCount(0);
				return true;
			}
			return false;
		}
	}
	return false;
}

void USkillSystemComponent::OnSkillPressed(FGameplayTag AbilityTag)
{
	//AuraAbilitySystemComponent->AbilityInputTagPressed(AbilityTag);
	
	if(!AbilityTag.IsValid() || bInSkillProtectTime)
		return;
	
	if(InputAbility(AbilityTag))
	{
		CurrentExecutingInputTag = AbilityTag;
		Character->GetCharacterMovement()->MaxWalkSpeed = MAXWALKSPEEDONSKILL;
	}
}

void USkillSystemComponent::OnSkillReleased(FGameplayTag AbilityTag)
{
	//AuraAbilitySystemComponent->AbilityInputTagReleased(AbilityTag);
	if(!AbilityTag.IsValid() || !CurrentExecutingInputTag.IsValid() || CurrentExecutingInputTag != AbilityTag || CurrentSkillState == ESkillState::None)  //这里是否需要验证tag需要考虑一下
		return;
	
	if(auto Spec = AuraAbilitySystemComponent->FindAbilitySpecFromHandle(SkillStateToAbilitySpecHandleMap[CurrentSkillState]))
	{
		AuraAbilitySystemComponent->AbilitySpecInputReleased(*Spec);
	}
	
	CurrentExecutingInputTag = FGameplayTag::EmptyTag;
}

void USkillSystemComponent::OnSkillHeld(FGameplayTag AbilityTag)
{
	//AuraAbilitySystemComponent->AbilityInputTagHeld(AbilityTag);
}

void USkillSystemComponent::StartSkillProtectTimer()
{
	if(bInSkillProtectTime)
	{
		UE_LOG(LogAura, Warning, TEXT("Skill Protect Timer already started"))
		return;
	}

	FTimerHandle SkillProtectTimer;
	FTimerDelegate SkillProtectTimerDelegate;
	SkillProtectTimerDelegate.BindLambda([this]()
	{
		bInSkillProtectTime = false;
	});
	GetWorld()->GetTimerManager().SetTimer(SkillProtectTimer, SkillProtectTimerDelegate, SkillProtectTime, false);
	bInSkillProtectTime = true;
}

void USkillSystemComponent::StartWaitFollowupTimer(float WaitFollowupTime)
{
	if(bInWaitFollowupTime)
	{
		UE_LOG(LogAura, Warning, TEXT("Wait Followup Timer already started"))
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(WaitFollowupTimer, WaitFollowupTimerDelegate, WaitFollowupTime, false);
	bInWaitFollowupTime = true;
}

bool USkillSystemComponent::TryActiveSkill(ESkillState SkillState)
{
	auto Spec = AuraAbilitySystemComponent->FindAbilitySpecFromHandle(SkillStateToAbilitySpecHandleMap[SkillState]);
	AuraAbilitySystemComponent->AbilitySpecInputPressed( *Spec);
	if(AuraAbilitySystemComponent->TryActivateAbility(Spec->Handle))
	{
		CurrentSkillState = SkillState;
		StartSkillProtectTimer();
		return true;
	}
	return false;
}


void USkillSystemComponent::OnFollowupTimerEnd()
{
	bInWaitFollowupTime = false;
	CurrentSkillState = ESkillState::None;
}

FGameplayAbilitySpec USkillSystemComponent::GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass)
{
	FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
	AuraAbilitySystemComponent->GiveAbility(AbilitySpec);
	return AbilitySpec;
}

















