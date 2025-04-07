// Copyright W.V. Koren. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "InputActionValue.h"

#include "PlayerSubmarine.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API APlayerSubmarine : public APawn
{
	GENERATED_BODY()

public:
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* Box;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SubmarineMesh;

	
	// Sets default values for this pawn's properties
	APlayerSubmarine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

// Enhanced Input System
	protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ElevateAction;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InteractAction;



	// Movement Functions
protected:

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Move(const FInputActionValue& InputValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Rotate(const FInputActionValue& InputValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Elevate(const FInputActionValue& InputValue);

	UFUNCTION(BlueprintNativeEvent, Category = "Movement")
	void Look(const FInputActionValue& InputValue);

	
	

	void InteractWithObject();



	float TorqueForce;

	float MoveForce;
	float ElevateForce;

	void StraightenBoatLevel();


	// UI

	protected:
	
	void TogglePauseMenu();

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UPauseScreenWidget> PauseMenuClass;

	//UPROPERTY(EditDefaultsOnly, Category = "UI")
	//TSubclassOf<class UShellWidget> ShellWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<class UGameHUDWidget> HUDWidgetClass;

	
	UPROPERTY()
	UUserWidget* PauseMenu;

	//UPROPERTY()
	//UUserWidget* ShellWidget;

	UPROPERTY()
	UUserWidget* HUDWidget;

	bool bIsPaused;


	// Oxygen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oxygen")
	float MaxOxygen;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oxygen")
	float CurrentOxygen;

	UPROPERTY(EditAnywhere, Category = "Oxygen")
	float OxygenDrainRate;

public:
	void RefillOxygen(float Amount);
	
};
