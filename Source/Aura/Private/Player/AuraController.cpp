// Copyright KhalorlStudios


#include "Player/AuraController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/IHighlightable.h"
#include <Input/AuraInputComponent.h>
#include <AbilitySystemBlueprintLibrary.h>
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "Components/SplineComponent.h"
#include <AuraGameplayTags.h>
#include <NavigationSystem.h>
#include "NavigationPath.h"

AAuraController::AAuraController()
{
	this->bReplicates = true;
	this->Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	this->CursorTrace();

	if (this->bAutoRunning)
	{
		if (APawn* controlledPawn = this->GetPawn())
		{
			const FVector locationOnSpline = this->Spline->FindLocationClosestToWorldLocation(controlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
			const FVector direction = this->Spline->FindDirectionClosestToWorldLocation(locationOnSpline, ESplineCoordinateSpace::World);
			controlledPawn->AddMovementInput(direction);

			const float distanceToDestination = (locationOnSpline - this->CachedDestination).Length();
			if (distanceToDestination <= this->AutoRunAcceptanceRadius)
			{
				this->bAutoRunning = false;
			}
		}
	}
}

void AAuraController::BeginPlay()
{
	Super::BeginPlay();

	check(this->AuraContext);

	if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		subsystem->AddMappingContext(this->AuraContext, 0);
	}
	
	this->bShowMouseCursor = true;
	this->DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI inputModeData;
	inputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputModeData.SetHideCursorDuringCapture(false);
	this->SetInputMode(inputModeData);
}

void AAuraController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* inputComponent = CastChecked<UAuraInputComponent>(this->InputComponent);
	inputComponent->BindAction(this->MoveAction, ETriggerEvent::Triggered, this, &AAuraController::Move);
	inputComponent->BindAction(this->ShiftAction, ETriggerEvent::Started, this, &AAuraController::ShiftPressed);
	inputComponent->BindAction(this->ShiftAction, ETriggerEvent::Completed, this, &AAuraController::ShiftReleased);
	inputComponent->BindAbilityActions(this->InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
}

void AAuraController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D inputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator rotation = this->GetControlRotation();
	const FRotator yawRotation(0.0, rotation.Yaw, 0.0);
	const FRotationMatrix rotationMatrix(FRotator(0.0, rotation.Yaw, 0.0));

	const FVector forwardDirection = rotationMatrix.GetUnitAxis(EAxis::X);
	const FVector rightDirection = rotationMatrix.GetUnitAxis(EAxis::Y);

	if (APawn* controlledPawn = this->GetPawn<APawn>())
	{
		controlledPawn->AddMovementInput(forwardDirection, inputAxisVector.Y);
		controlledPawn->AddMovementInput(rightDirection, inputAxisVector.X);
	}
}

void AAuraController::CursorTrace()
{
	this->GetHitResultUnderCursor(ECC_Visibility, false, this->CursorHit);
	if (!this->CursorHit.bBlockingHit) return;

	this->LastActor = this->ThisActor;
	this->ThisActor = this->CursorHit.GetActor();

	// Check for highlight
	if (this->ThisActor && this->ThisActor != this->LastActor)
	{
		this->ThisActor->Highlight();
	}

	// Check for unhighlight
	if (this->LastActor && this->LastActor != this->ThisActor)
	{
		this->LastActor->UnHighlight();
	}
}

void AAuraController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		this->bTargeting = this->ThisActor ? true : false;
		this->bAutoRunning = false;
	}
}

void AAuraController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	this->GetASC()->AbilityInputTagReleased(InputTag);
	if (!this->bTargeting && !this->bShiftKeyDown)
	{
		APawn* controlledPawn = this->GetPawn();
		if (this->FollowTime <= this->ShortPressThreshold && controlledPawn)
		{
			if (UNavigationPath* navPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, controlledPawn->GetActorLocation(), this->CachedDestination))
			{
				this->Spline->ClearSplinePoints();
				for (const FVector& pointLoc : navPath->PathPoints)
				{
					this->Spline->AddSplinePoint(pointLoc, ESplineCoordinateSpace::World);
				}
				this->bAutoRunning = true;
				// Makes sure destination is a movable locaiton
				this->CachedDestination = navPath->PathPoints[navPath->PathPoints.Num() - 1];
			}
		}
		this->FollowTime = 0.0;
		this->bTargeting = false;
	}
}

void AAuraController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (this->bShiftKeyDown || this->bTargeting || !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB))
	{
		if (this->GetASC())
		{
			this->GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		this->FollowTime += this->GetWorld()->GetDeltaSeconds();
		if (this->CursorHit.bBlockingHit)
		{
			this->CachedDestination = this->CursorHit.ImpactPoint;
		}
		if (APawn* controlledPawn = this->GetPawn())
		{
			const FVector worldDirection = (this->CachedDestination - controlledPawn->GetActorLocation()).GetSafeNormal();
			controlledPawn->AddMovementInput(worldDirection);
		}
	}
}

UAbilitySystemComponentBase* AAuraController::GetASC()
{
	if (!this->AuraAbilitySystemComponent)
	{
		this->AuraAbilitySystemComponent = Cast<UAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(this->GetPawn<APawn>()));

	}
	return this->AuraAbilitySystemComponent;
}
