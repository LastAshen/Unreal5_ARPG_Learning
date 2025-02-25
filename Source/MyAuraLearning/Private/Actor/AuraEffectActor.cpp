// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/AuraEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Components/SphereComponent.h"

AAuraEffectActor::AAuraEffectActor()
{

	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = MeshComponent;

	SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
	SphereComponent ->SetupAttachment(RootComponent);
	
}

void AAuraEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OnOverlapAction.Broadcast(OtherActor);
}

void AAuraEffectActor::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	
}

void AAuraEffectActor::SetHealth(AActor* SetActor)
{
	if(auto ASCInterface = Cast<IAbilitySystemInterface>(SetActor))
	{
		const auto AuraAttribute = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		auto MutableAuraAttribute = const_cast<UAuraAttributeSet*>(AuraAttribute);
		MutableAuraAttribute->SetHealth(AuraAttribute->GetHealth()+ 30.0f);

		Destroy();
	}
}

void AAuraEffectActor::SetMana(AActor* SetActor)
{
	if(auto ASCInterface = Cast<IAbilitySystemInterface>(SetActor))
	{
		const auto AuraAttribute = Cast<UAuraAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UAuraAttributeSet::StaticClass()));

		auto MutableAuraAttribute = const_cast<UAuraAttributeSet*>(AuraAttribute);
		MutableAuraAttribute->SetMana(AuraAttribute->GetMana()+ 20.0f);

		Destroy();
	}
}

void AAuraEffectActor::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AAuraEffectActor::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AAuraEffectActor::OnEndOverlap);
}

