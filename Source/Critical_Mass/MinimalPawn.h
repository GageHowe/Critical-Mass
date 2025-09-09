#pragma once

// base/example class for chaos mover - powered pawns

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MovementMode.h"
#include "MoverTypes.h"
#include "MoverComponent.h"
#include "MinimalPawn.generated.h"

UCLASS()
class CRITICAL_MASS_API AMinimalPawn : public APawn
{
	GENERATED_BODY()

public:
	AMinimalPawn();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	// virtual void AsyncPhysicsTickActor(float DeltaTime, float SimTime) override;
	// virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// UPROPERTY(VisibleAnywhere)
	// UMoverComponent* MoverComp;
};
