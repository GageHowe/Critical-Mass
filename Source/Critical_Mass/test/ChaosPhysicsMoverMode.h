// #pragma once
//
// #include "CoreMinimal.h"
// #include "MovementMode.h"
// #include "ChaosPhysicsMoverMode.generated.h"
//
// UCLASS()
// class UChaosPhysicsMoverMode : public UBaseMovementMode
// {
// 	GENERATED_BODY()
//
// public:
// 	UChaosPhysicsMoverMode(const FObjectInitializer& ObjectInitializer);
//
// protected:
// 	virtual void GenerateMove_Implementation(
// 		const FMoverTickStartData& StartState,
// 		const FMoverTimeStep& TimeStep,
// 		FProposedMove& OutProposedMove) const override;
//
// 	virtual void SimulationTick_Implementation(
// 		const FSimulationTickParams& Params,
// 		FMoverTickEndData& OutputState) override;
//
// private:
// 	UPROPERTY(EditDefaultsOnly, Category="ChaosPhysicsMover")
// 	float Thrust = 6000.f; // N (kg·cm/s²). Tweak for ship mass.
//
// 	UPROPERTY(EditDefaultsOnly, Category="ChaosPhysicsMover")
// 	float TorqueStrength = 4000.f; // N·m. Tweak for turning rate.
// };
