// Copyright KhalorlStudios


#include "AbilitySystem/AbilityTasks/TargetDataUnderMouse.h"
#include "AbilitySystemComponent.h"

UTargetDataUnderMouse* UTargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UTargetDataUnderMouse* MyObj = NewAbilityTask<UTargetDataUnderMouse>(OwningAbility);
	return MyObj;
}

void UTargetDataUnderMouse::SendMouseCursorData()
{
	// I think this generates the ScopedPredictionKey for the Replication call later
	FScopedPredictionWindow scopedPrediction(this->AbilitySystemComponent.Get());

	APlayerController* PC = this->Ability->GetCurrentActorInfo()->PlayerController.Get();
	FHitResult cursorHit;
	if (PC->GetHitResultUnderCursor(ECC_Visibility, false, cursorHit))
	{
		FGameplayAbilityTargetDataHandle dataHandle;
		FGameplayAbilityTargetData_SingleTargetHit* data = new FGameplayAbilityTargetData_SingleTargetHit();
		data->HitResult = cursorHit;
		dataHandle.Add(data);
		this->AbilitySystemComponent->ServerSetReplicatedTargetData(
			this->GetAbilitySpecHandle(), 
			this->GetActivationPredictionKey(), // Hopefully more explained about these prediction keys and how to determine which to use
			dataHandle, 
			FGameplayTag(), // Empty gameplay tag for the Application Tag ??
			this->AbilitySystemComponent->ScopedPredictionKey
		);
		if (this->ShouldBroadcastAbilityTaskDelegates())
		{
			this->ValidData.Broadcast(dataHandle);
		}
	}
}

void UTargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag ActivationTag)
{
	// Target data has been received, don't store it any longer
	this->AbilitySystemComponent->ConsumeClientReplicatedTargetData(this->GetAbilitySpecHandle(), this->GetActivationPredictionKey());
	if (this->ShouldBroadcastAbilityTaskDelegates())
	{
		this->ValidData.Broadcast(DataHandle);
	}
}

void UTargetDataUnderMouse::Activate()
{
	const bool bIsLocal = this->Ability->GetCurrentActorInfo()->IsLocallyControlled();
	if (bIsLocal)
	{
		this->SendMouseCursorData();
	}
	else
	{
		this->AbilitySystemComponent.Get()->AbilityTargetDataSetDelegate(this->GetAbilitySpecHandle(), this->GetActivationPredictionKey()).AddUObject(this, &UTargetDataUnderMouse::OnTargetDataReplicatedCallback);
		const bool bDelegateAlreadyCalled = this->AbilitySystemComponent.Get()->CallReplicatedTargetDataDelegatesIfSet(this->GetAbilitySpecHandle(), this->GetActivationPredictionKey());
		if (!bDelegateAlreadyCalled)
		{
			// Target data has not been set, need to wait for callback to be called first
			this->SetWaitingOnRemotePlayerData();
		}
	}	
}
