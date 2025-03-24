// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CPP_CharacterInteractionComponent.h"
#include "GameFramework/Pawn.h"
#include "Interact_Interface.h"
#include "InputActionValue.h"
#include "GameFramework/FloatingPawnMovement.h"
//#include "Interact_Interface.h"

#include "CPPSubmarineTest.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPPSubmarineTest : public APawn, public IInteract_Interface
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "True"))
	class UBoxComponent* Capsule;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	class USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MySceneRoot;
	
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* SubmarineMesh;			

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UFloatingPawnMovement* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "True"))
	UCPP_CharacterInteractionComponent* CharacterInteractionComponent;

public:
	// Sets default values for this pawn's properties
	ACPPSubmarineTest();


protected:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* RotateAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* ElevateAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* InteractAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	class UInputAction* FreeLookAction;

	
	// UPROPERTY(EditAnywhere, Category = "Rotation")
	// float RotationSpeed = 50.0f;  // Increased for smoother turning
	//
	// UPROPERTY(EditAnywhere, Category = "Rotation")
	// float RotationAcceleration = 5.0f;  // Increased for quicker response
	//
	// UPROPERTY(EditAnywhere, Category = "Rotation")
	// float RotationDeceleration = 2.5f;  // Smoother stopping  
	//
	// float CurrentRotationSpeed = 0.0f;

	// Angular velocity in degrees per second

	FRotator RotationVelocity;
	float CurrentPitch = 0.0f;
	FRotator SubmarinePitch;

	// Damping factor to reduce rotation over time
	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationDamping = 5.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float RotationSensitivity = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Rotation")
	float ReturnToCameraSpeed = 0.2f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveDeceleration = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveAcceleration = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MoveMaxSpeed = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ElevationSpeed = 0.4;


	bool bIsFreeLooking = false;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void BeginOverlap_Implementation(AActor* CausingActor) override;
	virtual void EndOverlap_Implementation(AActor* CausingActor) override;

	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	void Elevate(const FInputActionValue& InputValue);
	void InteractWithObject();
	void Move(const FInputActionValue& InputValue);
	void Rotate(const FInputActionValue& InputValue);
	void AdjustPitch();
	void Look(const FInputActionValue& InputValue);
	void FreeLook();
	void UnFreeLook();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float getMoveMaxSpeed();
	
	void RotateToCamera(float DeltaTime);

};

