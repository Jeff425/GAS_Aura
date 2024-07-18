// Copyright KhalorlStudios


#include "AbilitySystem/Data/CharacterClassInfo.h"

FCharacterClassDefaultInfo UCharacterClassInfo::GetClassDefaultInfo(ECharacterClass CharacterClass)
{
	return this->CharacterClassInformation.FindChecked(CharacterClass);
}
