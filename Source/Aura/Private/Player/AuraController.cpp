// Copyright KhalorlStudios


#include "Player/AuraController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/IHighlightable.h"

AAuraController::AAuraController()
{
	this->bReplicates = true;
}

void AAuraController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	this->CursorTrace();
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

	UEnhancedInputComponent* inputComponent = CastChecked<UEnhancedInputComponent>(this->InputComponent);
	inputComponent->BindAction(this->MoveAction, ETriggerEvent::Triggered, this, &AAuraController::Move);
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
	FHitResult cursorHit;
	this->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit);
	if (!cursorHit.bBlockingHit) return;

	this->LastActor = this->ThisActor;
	this->ThisActor = cursorHit.GetActor();

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
