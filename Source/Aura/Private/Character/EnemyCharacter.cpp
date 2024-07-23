// Copyright KhalorlStudios


#include "Character/EnemyCharacter.h"
#include "Aura/Aura.h"
#include "AbilitySystem/AbilitySystemComponentBase.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Components/WidgetComponent.h"
#include "AuraGameplayTags.h"
#include "GameFramework/CharacterMovementComponent.h"

// I did this in BP_EnemyBase
// But this->GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); Under the constructor can set the collision channel in code

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	this->GetCharacterMovement()->MaxWalkSpeed = this->BaseWalkSpeed;
	this->InitAbilityActorInfo();

	UAuraAbilitySystemLibrary::GiveStartupAbilities(this, this->AbilitySystemComponent);

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

int32 AEnemyCharacter::GetLevel()
{
	return this->Level;
}

// Registered to NewOrRemoved, so a positive value means it was added, 0 or less means it was removed
void AEnemyCharacter::HitReactTagChanged(const FGameplayTag CallbackTag, int32 NewCount)
{
	this->bHitReacting = NewCount > 0;
	this->GetCharacterMovement()->MaxWalkSpeed = this->bHitReacting ? 0.0 : this->BaseWalkSpeed;
}

void AEnemyCharacter::Die()
{
	this->SetLifeSpan(this->DeathLifeSpan);
	Super::Die();
}
