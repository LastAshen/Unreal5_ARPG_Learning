// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true; 
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	check(Subsystem);
	Subsystem->AddMappingContext(AuraContext,0);
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnchancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnchancedInputComponent->BindAction(MoveAction,ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
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
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);
	
	if (!HitResult.bBlockingHit) return;

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>( HitResult.GetActor());
	if (HitResult.bBlockingHit)
	{
		if (ThisActor)
		{
			if(LastActor)
			{
				if(LastActor != ThisActor)
				{
					ThisActor->Highlight();
					LastActor->UnHighlight();
				}
				else
				{
					//Do nothing
				}
			}
			else
			{
				ThisActor->Highlight();
			}
		}
		else
		{
			if(LastActor)
			{
				LastActor->UnHighlight();
			}
		}
	}
}



















