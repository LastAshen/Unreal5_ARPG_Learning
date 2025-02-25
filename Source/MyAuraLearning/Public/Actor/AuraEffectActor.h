// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraEffectActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAuraEffectActorOverlapSignature, AActor*, OtherActor);

class USphereComponent;

UCLASS()
class MYAURALEARNING_API AAuraEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAuraEffectActor();

	UFUNCTION()
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> MeshComponent;
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(BlueprintAssignable)
	FAuraEffectActorOverlapSignature OnOverlapAction;

	UFUNCTION(BlueprintCallable, Category = "AuraEffectActor")
	void SetHealth(AActor* SetActor);
	
	UFUNCTION(BlueprintCallable, Category = "AuraEffectActor")
	void SetMana(AActor* SetActor);
	
protected:
	virtual void BeginPlay() override;
};
