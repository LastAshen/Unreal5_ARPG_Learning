// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AuraGameplayTags.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Character.h"
#include "Input/AuraInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	//SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("SplineComponent"));
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if(Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext,0);
	}
	
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	SkillSystemComp = GetPawn()->FindComponentByClass<USkillSystemComponent>();

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent() 
{
	Super::SetupInputComponent();

	auto AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);
	AuraInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftKeyPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftKeyReleased);
	AuraInputComponent->BindAbilityActions(InputConfig, this,
		&AAuraPlayerController::AbilityInputTagPressed, &AAuraPlayerController::AbilityInputTagReleased, &AAuraPlayerController::AbilityInputHeld);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	//AutoRun();

	//人物转向鼠标方向
	if(CursorHit.bBlockingHit && bFacingCursor)
	{
		FVector LookDirection = (CursorHit.Location - GetPawn()->GetActorLocation()).GetSafeNormal();
		LookDirection = LookDirection.RotateAngleAxis(-90.f, FVector::UpVector); //角色是以Y轴为正方向，所以这里需要旋转90度做修正
		FRotator TragetRotation = FRotator(0.f, LookDirection.Rotation().Yaw, 0.f);
		auto ControlledCharacter = Cast<ACharacter>(GetPawn());
		ControlledCharacter->GetMesh()->SetWorldRotation(TragetRotation);

		// FRotator MeshRelativeRotation = ControlledCharacter->GetMesh()->GetRelativeRotation();
		// FString RotationString = FString::Printf(TEXT("Mesh相对旋转: Pitch=%.2f, Yaw=%.2f, Roll=%.2f"), 
		// 	MeshRelativeRotation.Pitch, 
		// 	MeshRelativeRotation.Yaw, 
		// 	MeshRelativeRotation.Roll);
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, RotationString);
	}
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bIsBlockedHit, bool bIsCriticalHit)
{
	if(IsValid(TargetCharacter) && DamageTextWidgetClass && IsLocalController())
	{
		auto DamageTextComp = NewObject<UDamageTextWidgetComponent>(TargetCharacter->GetRootComponent(), DamageTextWidgetClass, FName("DamageTextWidgetComp"));
		DamageTextComp->RegisterComponent();
		DamageTextComp->SetDamageText(DamageAmount, bIsBlockedHit, bIsCriticalHit);
		DamageTextComp->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextComp->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag Tag)
{
	// if(Tag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	// {
	// 	bTargeting = ThisActor ? true : false;
	// 	bAutoRunning = false;
	// }

	SkillSystemComp->OnSkillPressed(Tag);
}

void AAuraPlayerController::AbilityInputHeld(FGameplayTag Tag)
{
	SkillSystemComp->OnSkillHeld(Tag);

	// if(bTargeting || bIsShiftKeyDown)
	// {
	// 	if(GetGAS()) 
	// 		GetGAS()->AbilityInputTagHeld(Tag);
	// }
	// else
	// {
	// 	FollowTime += GetWorld()->GetDeltaSeconds();
	// 	
	// 	if(CursorHit.bBlockingHit)
	// 	{
	// 		CachedDestination = CursorHit.ImpactPoint;
	// 	}
	//
	// 	if(APawn* ControlledPawn = GetPawn())
	// 	{
	// 		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
	// 		ControlledPawn->AddMovementInput(WorldDirection, 1.f);
	// 	}
	// }
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag Tag)
{
	SkillSystemComp->OnSkillReleased(Tag);

	// if(!bTargeting && !bIsShiftKeyDown)
	// {
	// 	const APawn* ControlledPawn = GetPawn();
	// 	if(FollowTime <= ShortPressThreshold && ControlledPawn)
	// 	{
	// 		if(UNavigationPath* Path = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
	// 		{
	// 			SplineComponent->ClearSplinePoints();
	// 			for(FVector PathLocation : Path->PathPoints)
	// 			{
	// 				SplineComponent->AddSplinePoint(PathLocation, ESplineCoordinateSpace::World, true);
	// 				//DrawDebugSphere(GetWorld(), PathLocation, 10.f, 12, FColor::Green, false, -1, 0, 3.f);
	// 			}
	// 			bAutoRunning = true;
	// 			CachedDestination = Path->PathPoints.Last();
	// 		}
	// 	}
	//
	// 	FollowTime = 0.f;
	// 	bTargeting = false;
	// }
}


void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	//两种获取方向向量方式
	
	const FRotator  YawRotation(0.f, Rotation.Yaw, 0.f);
	 const FVector ForwardDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
	 const FVector RightDirection = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
	
	// const FVector ForwardDirection = Rotation.RotateVector(FVector::ForwardVector);
	// const FVector RightDirection = Rotation.RotateVector(FVector::RightVector);
	//end 两种获取方向向量方式
	
	auto MoveDirection = ForwardDirection * InputAxisVector.X + RightDirection * InputAxisVector.Y;
	auto ControlledPawn = GetPawn() ;
	if(ControlledPawn && MoveDirection!= FVector::ZeroVector)
	{
		//ControlledPawn->AddMovementInput(ForwardDirection * InputAxisValue.Y + RightDirection * InputAxisValue.X, 1.f);
		ControlledPawn->AddMovementInput(ForwardDirection, -InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, -InputAxisVector.X);
	}
}

void AAuraPlayerController::CursorTrace()
{
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>( CursorHit.GetActor());

	if(LastActor != ThisActor)
	{
		if(ThisActor) ThisActor->Highlight();
		if(LastActor) LastActor->UnHighlight();
	}
}

// UAuraAbilitySystemComponent* AAuraPlayerController::GetGAS()
// {
// 	if(!AuraAbilitySystemComponent)
// 	{
// 		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
// 	}
// 	return AuraAbilitySystemComponent;
// }

// void AAuraPlayerController::AutoRun()
// {
// 	if(bAutoRunning)
// 	{
// 		if(APawn* ControlledPawn = GetPawn())
// 		{
// 			const FVector LocationOnSpline = SplineComponent->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
// 			const FVector Direction = SplineComponent->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World); // ? 为什么？
// 			ControlledPawn->AddMovementInput(Direction, 1.f);
//
// 			const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
// 			if(DistanceToDestination < AutoRunAcceptRadius)
// 			{
// 				bAutoRunning = false;
// 			}
// 		}
// 	}
// }



















