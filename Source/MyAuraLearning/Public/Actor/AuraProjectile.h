// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "SphereDamageActor.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class UNiagaraSystem;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYAURALEARNING_API AAuraProjectile : public ASphereDamageActor
{
	GENERATED_BODY()
	
public:	//Functions
	AAuraProjectile();

	
public: //Properties
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;

	UPROPERTY()
	TObjectPtr<USceneComponent> HomingTargetSceneComp;
	
};



