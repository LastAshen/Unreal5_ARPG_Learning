// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SphereDamageActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "AbilitySystem/AuraAbilitySystem_BFL.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MyAuraLearning/MyAuraLearning.h"


ASphereDamageActor::ASphereDamageActor()
{
	PrimaryActorTick . bCanEverTick = false;
	bReplicates = true;
	
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionObjectType(ECC_Projectile);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_WorldStatic,ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	
}

void ASphereDamageActor::BeginPlay()
{
	Super::BeginPlay();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASphereDamageActor::OnSphereOverlap);
	AudioComp = UGameplayStatics::SpawnSoundAttached(LoopingSound, GetRootComponent());
	SetLifeSpan(LifeSpan);
}

void ASphereDamageActor::Destroyed()
{
	if(!bHit && !HasAuthority())
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f));
		if(AudioComp)
		{
			AudioComp->Stop();
			AudioComp->DestroyComponent();
		}
		bHit = true;
	}
	
	Super::Destroyed();
}

void ASphereDamageActor::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(!DamageEffectSpecHandle.Data.IsValid() ||DamageEffectSpecHandle.Data.Get()->GetContext().GetEffectCauser() == OtherActor)
		return;

	if(UAuraAbilitySystem_BFL::IsFriends(OtherActor, DamageEffectSpecHandle.Data->GetContext().GetEffectCauser()))
		return;

	if(!bHit)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation(), FRotator::ZeroRotator);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect, GetActorLocation(), FRotator::ZeroRotator, FVector(1.f));
		if(AudioComp)
		{
			AudioComp->Stop();
			AudioComp->DestroyComponent();
		}
		bHit = true;
	}
	
	if(HasAuthority())
	{
		if(auto TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
		{
			TargetASC -> ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
		}
		
		Destroy();
	}
	else
	{
		bHit = true;
	}
}


