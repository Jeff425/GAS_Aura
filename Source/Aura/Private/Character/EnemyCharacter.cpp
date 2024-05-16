// Copyright KhalorlStudios


#include "Character/EnemyCharacter.h"
#include "Aura/Aura.h"

// I did this in BP_EnemyBase
// But this->GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); Under the constructor can set the collision channel in code

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
