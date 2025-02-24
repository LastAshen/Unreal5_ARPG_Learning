// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "AuraEnemy.generated.h"

/**
 * 
 */
UCLASS()
class MYAURALEARNING_API AAuraEnemy : public AAuraCharacterBase, public IEnemyInterface
{
	GENERATED_BODY()

public:
	AAuraEnemy();

	// IEnemyInterface
	virtual void Highlight() override;
	virtual void UnHighlight() override;
	// End IEnemyInterface


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Aura Enemy")
	bool bIsHighlighted;

protected:
	virtual void BeginPlay() override;
};
