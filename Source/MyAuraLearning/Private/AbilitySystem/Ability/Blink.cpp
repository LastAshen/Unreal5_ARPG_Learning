// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/Blink.h"

#include "MyAuraLearning/AuraLogChannels.h"

FVector UBlink::GetBlinkLocation()
{
	auto Avatar = GetAvatarActorFromActorInfo();
	auto Forward = Avatar->GetActorForwardVector();

	FHitResult ForwardHitResult;
	FVector BlinkDirectionPoint;
	GetWorld()->LineTraceSingleByChannel(ForwardHitResult, Avatar->GetActorLocation(), Avatar->GetActorLocation() + Forward * BlinkDistance, ECollisionChannel::ECC_Visibility);
	if(ForwardHitResult.bBlockingHit)
	{
		BlinkDirectionPoint = ForwardHitResult.Location;
	}
	else
	{
		BlinkDirectionPoint = Avatar->GetActorLocation() + Forward * BlinkDistance;
	}
	
	//寻找在地面上的点
	FHitResult GroundHitResult;
	GetWorld()->LineTraceSingleByChannel(GroundHitResult, BlinkDirectionPoint + FVector(0, 0, 200.f), BlinkDirectionPoint - FVector(0.0f, 0.0f, 500.0f), ECollisionChannel::ECC_Visibility);
	if(GroundHitResult.bBlockingHit)
	{
		return GroundHitResult.Location;
	}

	UE_LOG(LogAura, Error, TEXT("Get Blink Location Failed!"));
	return FVector::ZeroVector;
}
