// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"

#include "Components/RadialSlider.h"

#include "GameHUDWidget.generated.h"


/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UGameHUDWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;



	UFUNCTION(BlueprintCallable)
	void UpdateShellCount(int32 Value);

	UFUNCTION(BlueprintCallable)
	void UpdateOxygen(float Current, float Max);
	

// protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXTShell;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	URadialSlider* OxygenRadial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	float OxygenPercent;


	
	
};
