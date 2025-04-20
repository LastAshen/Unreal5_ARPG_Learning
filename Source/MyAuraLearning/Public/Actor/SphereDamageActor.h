// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Actor.h"
#include "SphereDamageActor.generated.h"

class UNiagaraSystem;
class USphereComponent;

UCLASS()
class MYAURALEARNING_API ASphereDamageActor : public AActor
{
	GENERATED_BODY()

public:
	ASphereDamageActor();

	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn = true),Category = "Sphere Damage")
	FGameplayEffectSpecHandle DamageEffectSpecHandle;

protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sphere Damage")
	float LifeSpan = 10.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereComp;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	bool bHit = false;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> LoopingSound;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComp;

};
