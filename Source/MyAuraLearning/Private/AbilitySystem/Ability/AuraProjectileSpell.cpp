// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraProjectileSpell.h"

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

void UAuraProjectileSpell::SpawnProjectile(const FVector& TargetLocation)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if(!bIsServer) return;

	if(const auto CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo()))
	{
		const auto SocketLocation = CombatInterface->GetCombatSocketLocation();
		FTransform SpawnTransform;
		
		auto Avator = GetAvatarActorFromActorInfo();
		FRotator Rotation = (TargetLocation - Avator->GetActorLocation()).Rotation();

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
			
		auto Projectile = GetWorld()->SpawnActorDeferred<AAuraProjectile>(ProjectileClass,SpawnTransform,
			GetOwningActorFromActorInfo(),Cast<APawn>(GetOwningActorFromActorInfo()),ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
		
		Projectile->FinishSpawning(SpawnTransform);
	}
}
