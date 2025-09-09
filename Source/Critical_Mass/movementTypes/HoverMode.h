// // HoverMode.h
// #pragma once
//
// #include "CoreMinimal.h"
// #include "MovementMode.h"
// #include "HoverMode.generated.h"
//
// UCLASS()
// class CRITICAL_MASS_API UHoverMode : public UBaseMovementMode
// {
// 	GENERATED_BODY()
//
// public:
// 	virtual void GenerateMove_Implementation(
// 		const FMoverTickStartData& StartState,
// 		const FMoverTimeStep& TimeStep,
// 		FProposedMove& OutProposedMove
// 	) const override;
//
// 	virtual void SimulationTick_Implementation(
// 		const FSimulationTickParams& Params,
// 		FMoverTickEndData& OutputState
// 	) override;
// };
