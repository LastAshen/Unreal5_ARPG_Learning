// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AuraWidgetController.h"
#include "GameplayTagContainer.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "OverlayWidgetController.generated.h"

class USkillSystemComponent;
class UAuraAbilitySystemComponent;
struct FOnAttributeChangeData;
class UAuraUserWidget;

USTRUCT(BlueprintType)
struct FUIWidgetRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FGameplayTag MessageTag = FGameplayTag();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText MessageText = FText();

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UAuraUserWidget> MessageWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Image = nullptr;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributeChangedSignature, float, NewValue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMessageWidgetRowSignature, FUIWidgetRow, Row);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbilityInfoSignature, const FAuraAbilityInfo&, Info);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSkillIntChangedSignature, int32, NewValue, int32, MaxValue);


UCLASS(Blueprintable, BlueprintType)
class MYAURALEARNING_API UOverlayWidgetController : public UAuraWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependecies() override;
	
	virtual void SetWidgetControllerParams(const FWidgetControllerParams& InParams) override;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnHealthChanged;

	UPROPERTY(BlueprintAssignable, Category="GAS|Attributes")
	FOnAttributeChangedSignature OnMaxHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Attributes")
	FOnAttributeChangedSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "GAS|Messages")
	FMessageWidgetRowSignature MessageWidgetRowDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Skills")
	FOnSkillIntChangedSignature OnSkillChargePointCountChangedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Skills")
	FOnSkillIntChangedSignature OnSkillEnergyPointCountChangedDelegate;
	
protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data"	)
	TObjectPtr<UDataTable> MessageDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Data"	)
	TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly)
	FAbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY()
	TObjectPtr<USkillSystemComponent> SkillSystemComp;
	
	template<typename T> 
	T* GetDataTableRowByTag(UDataTable* DataTable,const FGameplayTag& MessageTag);

	void OnInitializeStartupAbilities(UAuraAbilitySystemComponent* ASC);
};

template <typename T>
T* UOverlayWidgetController::GetDataTableRowByTag(UDataTable* DataTable, const FGameplayTag& MessageTag)
{
	return DataTable->FindRow<T>(MessageTag.GetTagName(), TEXT(""));
} 
















