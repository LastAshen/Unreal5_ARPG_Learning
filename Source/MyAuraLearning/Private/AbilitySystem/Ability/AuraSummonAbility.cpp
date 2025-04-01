// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Ability/AuraSummonAbility.h"

// #include "AI/NavigationSystemBase.h"
// #include "Kismet/KismetSystemLibrary.h"

TArray<FVector> UAuraSummonAbility::GetSpawnLocations()
{
	auto Result = TArray<FVector>();
	const FVector ForwardVector = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	const float DeltaSpread = SpawnSpread / NumMinions;

	const FVector RightOfSpread = ForwardVector.RotateAngleAxis(SpawnSpread / 2.0f, FVector::UpVector);
	const FVector LeftOfSpread = ForwardVector.RotateAngleAxis(-SpawnSpread / 2.0f, FVector::UpVector);

	for(int i = 0; i < NumMinions; i++)
	{
		const FVector Direction  =  LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
		// auto EndLocation = Location + Direction * MaxSpawnDistance;
		// auto StartLocation = Location + Direction * MinSpawnDistance;
		// UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), StartLocation, EndLocation, 4.f, FLinearColor::Green, 2.f);
		// DrawDebugSphere(GetWorld(), StartLocation, 10.f, 12, FColor::Orange, false, 2.f);  
		// DrawDebugSphere(GetWorld(), EndLocation, 10.f, 12, FColor::Orange, false, 2.f);

		auto RandLocation =Location + Direction * FMath::RandRange(MinSpawnDistance, MaxSpawnDistance);
		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, RandLocation + FVector(0.f, 0.f, 200.f), RandLocation - FVector(0.f, 0.f, 200.f), ECollisionChannel::ECC_Visibility);
		if(HitResult.IsValidBlockingHit())
		{
			RandLocation = HitResult.ImpactPoint;
		}
		Result.Add(RandLocation);
		//DrawDebugSphere(GetWorld(), RandLocation, 10.f, 12, FColor::Blue, false, 2.f);
	}
	
	return Result;
}
















