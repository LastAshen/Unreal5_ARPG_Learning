// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class MYAURALEARNING_API AAuraProjectile : public AActor
{
	GENERATED_BODY()
	
public:	//Functions
	AAuraProjectile();



protected: //Functions
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
private: //Functions;

	
public: //Properties
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovementComp;
	
protected: //Properties
	
private:// Properties
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComp;
};



