#pragma once

#include "CoreMinimal.h"
#include "MovementMode.h"                // UBaseMovementMode
#include "MoverSimulationTypes.h"        // FMoverProposed / FMoverDefaultSyncState types
#include "ChaosPhysicsMoverMode.generated.h"

UCLASS()
class UChaosPhysicsMoverMode : public UBaseMovementMode
{
	GENERATED_BODY()

public:
	UChaosPhysicsMoverMode(const FObjectInitializer& ObjectInitializer);

protected:
	// GenerateMove: build desired linear/angular velocities from inputs
	virtual void GenerateMove_Implementation(
		const FMoverTickStartData& StartState,
		const FMoverTimeStep& TimeStep,
		FProposedMove& OutProposedMove) const override;

	// SimulationTick: apply move to physics component and capture real physics state back into sync state
	virtual void SimulationTick_Implementation(
		const FSimulationTickParams& Params,
		FMoverTickEndData& OutputState) override;

private:
	// Tunables
	UPROPERTY(EditDefaultsOnly, Category = "ChaosPhysicsMover")
	float MaxThrust = 4000.0f; // Unreal units (force)/tunable: used when converting input to velocity/force

	UPROPERTY(EditDefaultsOnly, Category = "ChaosPhysicsMover")
	bool bUseSetVelocityDirectly = true; // if true we call SetPhysicsLinearVelocity; else you can switch to AddForce path
};
