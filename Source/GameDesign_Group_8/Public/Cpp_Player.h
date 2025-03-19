// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Cpp_Player.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACpp_Player : public APawn
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	class UBoxComponent* Capsule;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	UStaticMeshComponent* SubmarineMesh;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess=true))
	UFloatingPawnMovement* MovementComponent;




public:
	// Sets default values for this pawn's properties
	ACpp_Player();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveForwardAction;
	
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveUpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* TurnAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

FRotator RotationVelocity;


protected:

	void MoveForward(const FInputActionValue& InputValue);
	void MoveUp(const FInputActionValue& InputValue);
	void Turn(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	
	
};
