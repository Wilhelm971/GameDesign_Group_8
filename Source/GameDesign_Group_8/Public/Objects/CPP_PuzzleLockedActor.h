#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Puzzles/CPP_PuzzleManagerBase.h"
#include "CPP_PuzzleLockedActor.generated.h"

UCLASS()
class GAMEDESIGN_GROUP_8_API ACPP_PuzzleLockedActor : public AActor
{
	GENERATED_BODY()

	FVector StartingLocation;
	FVector EndingLocation;

	bool bCanOpen = false;

	UPROPERTY(EditAnywhere, Category = "Opening Parameters")
	float OpeningSpeed = 1.0f;

	UPROPERTY(EditAnywhere, Category = "Opening Parameters")
	float MoveHeight = 100.0f;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (AllowPrivateAccess = "True"))
	UStaticMeshComponent* StaticMesh;

public:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "References")
	ACPP_PuzzleManagerBase* LinkedPuzzleManager;
	
	ACPP_PuzzleLockedActor();

protected:
	
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Puzzle")
	void PuzzleSolved(); 

};
