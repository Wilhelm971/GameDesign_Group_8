// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/UMG.h"
#include "ShellWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEDESIGN_GROUP_8_API UShellWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UShellWidget(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateShellCount(int32 Value);

UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* TXTShell;


	
};
