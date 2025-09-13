// #include "ChaosPhysicsMoverMode.h"
// #include "MoverComponent.h"
// // #include "MoveLibrary/CommonMovementStructs.h"
// #include "Components/PrimitiveComponent.h"
//
// UChaosPhysicsMoverMode::UChaosPhysicsMoverMode(const FObjectInitializer& ObjectInitializer)
// 	: Super(ObjectInitializer)
// {
// 	// allow async ticking since we’re delegating to Chaos
// 	bSupportsAsync = true;
// }
//
// void UChaosPhysicsMoverMode::GenerateMove_Implementation(
// 	const FMoverTickStartData& StartState,
// 	const FMoverTimeStep& TimeStep,
// 	FProposedMove& OutProposedMove) const
// {
// 	OutProposedMove.LinearVelocity = FVector::ZeroVector;
// 	OutProposedMove.AngularVelocity = FRotator::ZeroRotator;
//
// 	// grab typical inputs
// 	if (const FCharacterDefaultInputs* Inputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>())
// 	{
// 		OutProposedMove.LinearVelocity = Inputs->GetMoveInput_WorldSpace(); // normalized direction
// 		OutProposedMove.AngularVelocity = Inputs->OrientationIntent;        // rot intent
// 	}
// }
//
// void UChaosPhysicsMoverMode::SimulationTick_Implementation(
// 	const FSimulationTickParams& Params,
// 	FMoverTickEndData& OutputState)
// {
// 	const FMovingComponentSet& Moving = Params.MovingComps;
// 	UPrimitiveComponent* Prim = Moving.UpdatedPrimitive.Get();
//
// 	if (!Prim || !Prim->IsSimulatingPhysics())
// 	{
// 		OutputState.SyncState = Params.StartState.SyncState;
// 		OutputState.AuxState = Params.StartState.AuxState;
// 		return;
// 	}
//
// 	const float Dt = Params.TimeStep.StepMs * 0.001f;
//
// 	// apply force in world space
// 	const FVector Force = Params.ProposedMove.LinearVelocity.GetClampedToMaxSize(1.f) * Thrust;
// 	Prim->AddForce(Force, NAME_None, true);
//
// 	// apply torque for rotation
// 	const FVector Torque(
// 		Params.ProposedMove.AngularVelocity.Roll * TorqueStrength,
// 		Params.ProposedMove.AngularVelocity.Pitch * TorqueStrength,
// 		Params.ProposedMove.AngularVelocity.Yaw * TorqueStrength);
// 	Prim->AddTorqueInDegrees(Torque, NAME_None, true);
//
// 	// sync back state
// 	FMoverDefaultSyncState& Sync =
// 		OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();
//
// 	Sync.SetTransforms_WorldSpace(
// 		Prim->GetComponentLocation(),
// 		Prim->GetComponentRotation(),
// 		Prim->GetComponentVelocity(),
// 		nullptr);
//
// 	OutputState.AuxState = Params.StartState.AuxState;
// 	OutputState.MovementEndState.ResetToDefaults();
// }
