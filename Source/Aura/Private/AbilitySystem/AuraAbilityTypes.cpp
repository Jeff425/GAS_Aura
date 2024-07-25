// Copyright KhalorlStudios


#include "AbilitySystem/AuraAbilityTypes.h"

bool FAuraGameplayEffectContext::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	// Technically I am only using 8 bits right now
	uint32 repBits = 0;
	if (Ar.IsSaving())
	{
		if (this->bReplicateInstigator && this->Instigator.IsValid())
		{
			repBits |= 1 << 0;
		}
		if (this->bReplicateEffectCauser && this->EffectCauser.IsValid())
		{
			repBits |= 1 << 1;
		}
		if (this->AbilityCDO.IsValid())
		{
			repBits |= 1 << 2;
		}
		if (this->bReplicateSourceObject && this->SourceObject.IsValid())
		{
			repBits |= 1 << 3;
		}
		if (this->Actors.Num() > 0)
		{
			repBits |= 1 << 4;
		}
		if (this->HitResult.IsValid())
		{
			repBits |= 1 << 5;
		}
		if (this->bHasWorldOrigin)
		{
			repBits |= 1 << 6;
		}
		if (this->bIsCriticalHit)
		{
			repBits |= 1 << 7;
		}
	}

	Ar.SerializeBits(&repBits, 8);

	if (repBits & (1 << 0))
	{
		Ar << this->Instigator;
	}
	if (repBits & (1 << 1))
	{
		Ar << this->EffectCauser;
	}
	if (repBits & (1 << 2))
	{
		Ar << this->AbilityCDO;
	}
	if (repBits & (1 << 3))
	{
		Ar << this->SourceObject;
	}
	if (repBits & (1 << 4))
	{
		SafeNetSerializeTArray_Default<31>(Ar, Actors);
	}
	if (repBits & (1 << 5))
	{
		if (Ar.IsLoading())
		{
			if (!this->HitResult.IsValid())
			{
				this->HitResult = TSharedPtr<FHitResult>(new FHitResult());
			}
		}
		this->HitResult->NetSerialize(Ar, Map, bOutSuccess);
	}
	if (repBits & (1 << 6))
	{
		Ar << WorldOrigin;
		this->bHasWorldOrigin = true;
	}
	else
	{
		this->bHasWorldOrigin = false;
	}
	if (repBits & (1 << 7))
	{
		// Sir steven copied above, but the bit did not get packed into Ar, only the actual data
		//Ar << bIsCriticalHit;
		this->bIsCriticalHit = true;
	}
	else {
		this->bIsCriticalHit = false;
	}

	if (Ar.IsLoading())
	{
		this->AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
	}

	bOutSuccess = true;
	return true;
}
