// #include "HoverMode.h"
//
// void UHoverMode::GenerateMove_Implementation(
// 	const FMoverTickStartData& StartState,
// 	const FMoverTimeStep& TimeStep,
// 	FProposedMove& OutProposedMove) const
// {
// 	// Example: always move forward in +X
// 	OutProposedMove.LinearVelocity = FVector(600.f, 0.f, 0.f);
// }
//
// void UHoverMode::SimulationTick_Implementation(
// 	const FSimulationTickParams& Params,
// 	FMoverTickEndData& OutputState)
// {
// 	// Advance position manually
// 	OutputState.Location = Params.StartState.Location +
// 						   Params.ProposedMove.LinearVelocity * Params.TimeStep.StepMs * 0.001f;
// 	OutputState.LinearVelocity = Params.ProposedMove.LinearVelocity;
// }
