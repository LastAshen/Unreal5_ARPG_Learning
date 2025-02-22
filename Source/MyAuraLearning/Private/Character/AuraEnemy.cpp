// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AuraEnemy.h"

#include "MyAuraLearning/MyAuraLearning.h"

AAuraEnemy::AAuraEnemy()
{
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
}

void AAuraEnemy::Highlight()
{
	bIsHighlighted = true;
	GetMesh()->SetRenderCustomDepth(true);
	GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);

	//set weapon highlight
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(true);
		Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AAuraEnemy::UnHighlight()
{
	bIsHighlighted = false;
	GetMesh()->SetRenderCustomDepth(false);

	//set weapon unhighlight
	if (Weapon)
	{
		Weapon->SetRenderCustomDepth(false);
	}
}
