// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/SkillSystemComponent.h"
#include "Interaction/CharacterSkillInterface.h"
#include "UI/Widget/DamageTextWidgetComponent.h"
#include "AuraPlayerController.generated.h"

class UAuraInputConfig;
struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
class IEnemyInterface;
class USplineComponent;
/**
 * 
 */
UCLASS()
class MYAURALEARNING_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AAuraPlayerController();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit);

	UFUNCTION(BlueprintCallable)
	FHitResult GetCursorHitResult(){return CursorHit;};

	UFUNCTION(BlueprintCallable)
	void SetFacingCursor(bool bNewFacingCursor){bFacingCursor = bNewFacingCursor;};

	UFUNCTION(BlueprintCallable)
	bool GetFacingCursor(){ return bFacingCursor;};
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, Category = "Skill System")
	TObjectPtr<USkillSystemComponent> SkillSystemComp;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;
	
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UAuraInputConfig> InputConfig;

	void AbilityInputTagPressed(FGameplayTag Tag);
	void AbilityInputTagReleased(FGameplayTag Tag);
	void AbilityInputHeld(FGameplayTag Tag);

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	bool bIsShiftKeyDown = false;
	void ShiftKeyPressed(){bIsShiftKeyDown = true;}
	void ShiftKeyReleased(){bIsShiftKeyDown = false;}

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;
	
	//Movement
	FHitResult CursorHit;
	FVector CachedDestination;
	float FollowTime= 0.f;
	float ShortPressThreshold = 0.3f;
	bool bAutoRunning = false;
	bool bTargeting = false;
	bool bFacingCursor = true;
	
	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptRadius = 50.f;

	// UPROPERTY(VisibleAnywhere)
	// TObjectPtr<USplineComponent> SplineComponent;

	//void AutoRun();
	//end Movement

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextWidgetComponent> DamageTextWidgetClass;
};
























