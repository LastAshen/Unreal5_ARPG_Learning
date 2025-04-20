// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AuraGameplayTags.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Interaction/CombatInterface.h"

//#include "Kismet/KismetSystemLibrary.h"

void UAuraProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                           const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                           const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	//UKismetSystemLibrary::PrintString(this, FString("ActivateAbility (C++)"), true, true, FColor::Green, 3);

	if(HasAuthority(&ActivationInfo))
	{
		
	}
}

AAuraProjectile* UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation, bool OverridePitch, float Pitch, AActor* HomingTarget, const FGameplayTag& FireSocketTag)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return nullptr;

	const auto SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(),
		FireSocketTag.IsValid() ? FireSocketTag : FAuraGameplayTags::Get().Montage_Attack_Weapon);

	
	FTransform SpawnTransform;
	FRotator Rotation = (TargetLocation - SocketLocation).Rotation();
	if(OverridePitch)
		Rotation.Pitch = Pitch;

	SpawnTransform.SetLocation(SocketLocation);
	SpawnTransform.SetRotation(Rotation.Quaternion());
		
	const auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,
		GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	const auto SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), SourceASC->MakeEffectContext());


	FAuraGameplayTags GameplayTags = FAuraGameplayTags::Get();
	for(auto& Pair :DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	if(HomingTarget)
	{
		if(HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComp->HomingTargetComponent = HomingTarget->GetRootComponent(); // 这里HomingTargetComponent是TWeakObjectPtr<UCombatComponent>类型
		}
		else
		{
			//没有对象的话，创建一个虚拟的目标
			Projectile->HomingTargetSceneComp = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComp->SetWorldLocation(TargetLocation);
			Projectile->ProjectileMovementComp->HomingTargetComponent = Projectile->HomingTargetSceneComp; 
		}
		Projectile->ProjectileMovementComp->HomingAccelerationMagnitude = 3000.f;
		Projectile->ProjectileMovementComp->bIsHomingProjectile = true;
	}
	else
	{
		Projectile->ProjectileMovementComp->bIsHomingProjectile = false;
	}

	//react
	//UAbilitySystemBlueprintLibrary::Assi
	//UAbilitySystemBlueprintLibrary::(SpecHandle, Pair.Key, ScaledDamage);

	Projectile->DamageEffectSpecHandle = SpecHandle;
	Projectile->FinishSpawning(SpawnTransform);
	return Projectile;
}





















