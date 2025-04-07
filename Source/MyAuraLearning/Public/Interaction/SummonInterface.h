// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SummonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI,BlueprintType)
class USummonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MYAURALEARNING_API ISummonInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SaveMinionRef(AActor* MinionRef);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	int32 GetNumMinions() const;
};
































