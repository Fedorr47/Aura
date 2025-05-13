// Copyright - none


#include "Input/AuraInputConfig.h"

#include "Aura/AuraLogChannels.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionByTag(const FGameplayTag& InputTag, bool bLogNotFound) const
{
	for (FAuraInputAction InputAction : AbilityInputActions)
	{
		if (InputAction.InputAction && InputAction.InputTag == InputTag)
		{
			return InputAction.InputAction;
		}
	}
	
	if (bLogNotFound)
	{
		UE_LOG(LogAura, Warning, TEXT("Can't find AbilityInputAction for InputTag [%s] on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}
	
	return nullptr;
}
