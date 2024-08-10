// Copyright KhalorlStudios


#include "AbilitySystem/Data/CharacterClassInfo.h"

const FCharacterClassDefaultInfo& UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return this->CharacterClassInformation.FindChecked(CharacterClass);
}
