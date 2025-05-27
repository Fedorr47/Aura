// Copyright - none

#pragma once

#include "CoreMinimal.h"

#include <initializer_list>

#include "AuraAbilityTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/AuraPlayerState.h"
#include "UI/HUD/AuraHUD.h"
#include "Data/CharacterClassInfo.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "GameplayTagsManager.h"
#include "AuraAbilitySystemLibrary.generated.h"

class UAbilityInfo;

/**
 * 
 */
UCLASS()
class AURA_API UAuraAbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UOverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static UAttributeMenuWidgetController* GetAttributeWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|WidgetController", meta = (DefaultToSelf = "WorldContextObject"))
	static USpellMenuWidgetController* GetSpellMenuWidgetController(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void InitializeDefaultAttributes(
		const UObject* WorldContextObject,
		ECharacterClass CharacterClass,
		float Level,
		UAbilitySystemComponent* ASC);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static void GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass = ECharacterClass::Magic);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UCharacterClassInfo* GetCharacterClassInfo(const UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|CharacterClassDefaults")
	static UAbilityInfo* GetAbilityInfo(const UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsBlockedHit(const FGameplayEffectContextHandle& ContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetBlockedHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsBlockedHit);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static bool IsCriticalHit(const FGameplayEffectContextHandle& ContextHandle);
	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayEffects")
	static void SetCriticalHit(UPARAM(ref) FGameplayEffectContextHandle& ContextHandle, bool bInIsCriticalHit);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayUtils")
	static void GetLivePlayersWithRadius(
		const UObject* WorldContextObject,
		TArray<AActor*>& OutOverlappingActors,
		const TArray<AActor*>& ActorsToIgnore,
		float Radius,
		const FVector& SphereOrigin);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayUtils")
	static bool IsNotFriend(AActor* FirstActor, AActor* SecondActor);

	UFUNCTION(BlueprintPure, Category = "AuraAbilitySystemLibrary|GameplayUtils")
	static int32 GetReward(const UObject* WorldContextObject, ECharacterClass InCharacterClass, int32 InLevel);

	UFUNCTION(BlueprintCallable, Category = "AuraAbilitySystemLibrary|GameplayUtils|Damage")
	static FGameplayEffectContextHandle DoDamage(const FDamageEffectParam& DamageEffectParam);

	template <typename TController>
	static TController* GetWidgetController(const UObject* WorldContextObject, TController* (AAuraHUD::*Getter)(const FWidgetControllerParams&))
	{
		if (!IsValid(WorldContextObject)) return nullptr;

		if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
		{
			if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
			{
				if (AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>())
				{
					UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
					UAttributeSet* AS = PS->GetAttributeSet();
					const FWidgetControllerParams Params(PC, PS, ASC, AS);
					return (AuraHUD->*Getter)(Params);
				}
			}
		}
		return nullptr;
	}
	
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
