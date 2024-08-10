// Copyright KhalorlStudios


#include "Character/EnemyCharacter.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/AuraAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"

// I did this in BP_EnemyBase
// But this->GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); Under the constructor can set the collision channel in code

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->GetCharacterMovement()->MaxWalkSpeed = this->BaseWalkSpeed;
	this->InitAbilityActorInfo();

	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, this->AbilitySystemComponent, this->CharacterClass);

	if (UBaseUserWidget* auraUserWidget = Cast<UBaseUserWidget>(this->HealthBar->GetUserWidgetObject()))
	{
		auraUserWidget->SetWidgetController(this);
	}
	
	UAuraAttributeSet* auraAS = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			this->OnHealthChanged.Broadcast(Data.NewValue);
		}
	);
	this->AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(auraAS->GetMaxHealthAttribute()).AddLambda(
		[this](const FOnAttributeChangeData& Data) {
			this->OnMaxHealthChanged.Broadcast(Data.NewValue);
		}
	);

	this->AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Effects_HitReact, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AEnemyCharacter::HitReactTagChanged);

	this->OnHealthChanged.Broadcast(auraAS->GetHealth());
	this->OnMaxHealthChanged.Broadcast(auraAS->GetMaxHealth());
}

void AEnemyCharacter::InitializeDefaultAttributes() const
{
	UAuraAbilitySystemLibrary::InitializeDefaultAttributes(this, this->CharacterClass, this->Level, this->AbilitySystemComponent);
}

AEnemyCharacter::AEnemyCharacter()
{
	this->AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponentBase>("AbilitySystemComponent");
	this->AbilitySystemComponent->SetIsReplicated(true);
	this->AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	this->AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");

	this->HealthBar = CreateDefaultSubobject<UWidgetComponent>("HealthBar");
	this->HealthBar->SetupAttachment(this->GetRootComponent());

	// AI will not snap. Can change rotation speed on the character movement
	this->bUseControllerRotationPitch = false;
	this->bUseControllerRotationRoll = false;
	this->bUseControllerRotationYaw = false;
	this->GetCharacterMovement()->bUseControllerDesiredRotation = true;

	this->PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::Highlight()
{

	this->GetMesh()->SetRenderCustomDepth(true);
	this->GetMesh()->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	if (this->Weapon) {
		this->Weapon->SetRenderCustomDepth(true);
		this->Weapon->SetCustomDepthStencilValue(CUSTOM_DEPTH_RED);
	}
}

void AEnemyCharacter::UnHighlight()
{
	this->GetMesh()->SetRenderCustomDepth(false);
	if (this->Weapon) {
		this->Weapon->SetRenderCustomDepth(false);
	}
}

void AEnemyCharacter::InitAbilityActorInfo()
{
	this->AbilitySystemComponent->InitAbilityActorInfo(this, this);
	// Both this and Aura character are calling this. Feels like there's a better way...
	Cast<UAbilitySystemComponentBase>(this->AbilitySystemComponent)->AbilityActorInfoSet();

	this->InitializeDefaultAttributes();
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	UAuraAttributeSet* auraAS = CastChecked<UAuraAttributeSet>(this->AttributeSet);
	if (auraAS->GetMaxHealth() > 0)
	{
		this->PrimaryActorTick.bCanEverTick = false;
		this->OnHealthChanged.Broadcast(auraAS->GetHealth());
		this->OnMaxHealthChanged.Broadcast(auraAS->GetMaxHealth());
	}
}

int32 AEnemyCharacter::GetLevel()
{
	return this->Level;
}

// Registered to NewOrRemoved, so a positive value means it was added, 0 or less means it was removed
void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	this->bHitReacting = NewCount > 0;
	this->GetCharacterMovement()->MaxWalkSpeed = this->bHitReacting ? 0.0 : this->BaseWalkSpeed;
	// Update the AI Blackboard key
	if (this->AuraAIController && this->AuraAIController->GetBlackboardComponent())
	{
		this->AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), this->bHitReacting);
	}
}

void AEnemyCharacter::Die()
{
	this->SetLifeSpan(this->DeathLifeSpan);
	Super::Die();
}

void AEnemyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	if (!this->HasAuthority()) return;
	this->AuraAIController = Cast<AAuraAIController>(NewController);

	this->AuraAIController->GetBlackboardComponent()->InitializeBlackboard(*this->BehaviorTree->BlackboardAsset);
	this->AuraAIController->RunBehaviorTree(this->BehaviorTree);

	this->AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("HitReacting"), false);

	// Both Ranger and Elementalist are ranged attackers
	this->AuraAIController->GetBlackboardComponent()->SetValueAsBool(FName("RangedAttacker"), this->CharacterClass != ECharacterClass::Warrior);
}

void AEnemyCharacter::SetCombatTarget_Implementation(AActor* InCombatTarget)
{
	this->CombatTarget = InCombatTarget;
}

AActor* AEnemyCharacter::GetCombatTarget_Implementation() const
{
	return this->CombatTarget;
}
