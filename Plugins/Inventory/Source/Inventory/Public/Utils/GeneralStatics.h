#pragma once

#include "CoreMinimal.h"

struct INVENTORY_API FGeneralStatics
{
	template<typename... Parts>
	static FName MakeTagName(Parts&&... parts)
	{
		FString TagString;
		((TagString += FString(parts) + TEXT(".")), ...);
		TagString.RemoveFromEnd(TEXT("."));
		return FName(TagString);
	}

	template<typename... Parts>
	static FGameplayTag AddInputTag(const FString& TagComment, Parts&&... parts)
	{
		return UGameplayTagsManager::Get().AddNativeGameplayTag(
			MakeTagName(Forward<Parts>(parts)...),
			TagComment
		);
	}
};