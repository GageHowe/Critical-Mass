#include "ChaosPhysicsMoverMode.h"

#include "MoverComponent.h"
#include "MoveLibrary/MovementUtils.h"          // optional helpers
#include "MoveLibrary/CommonMovementStructs.h"  // FCharacterDefaultInputs (project may have it here)
#include "Components/PrimitiveComponent.h"
#include "PhysicsEngine/BodyInstance.h"
#include "GameFramework/Actor.h"

UChaosPhysicsMoverMode::UChaosPhysicsMoverMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// tag if you want: GameplayTags.AddTag(...)
	bSupportsAsync = false;
}

void UChaosPhysicsMoverMode::GenerateMove_Implementation(
	const FMoverTickStartData& StartState,
	const FMoverTimeStep& TimeStep,
	FProposedMove& OutProposedMove) const
{
	// Default: zero everything
	OutProposedMove.LinearVelocity = FVector::ZeroVector;
	OutProposedMove.AngularVelocity = FRotator::ZeroRotator;
	OutProposedMove.bHasDirIntent = false;

	// Pull typical character-style inputs (same pattern as UE moving modes)
	const FCharacterDefaultInputs* CharInputs = StartState.InputCmd.InputCollection.FindDataByType<FCharacterDefaultInputs>();
	if (!CharInputs)
	{
		return;
	}

	// Example mapping:
	// - CharInputs->GetMoveInput_WorldSpace() returns a vector (x forward, y right)
	// - CharInputs->GetOrientationIntentDir_WorldSpace() gives orientation intent
	const FVector MoveInputWorld = CharInputs->GetMoveInput_WorldSpace(); // normalized or magnitude per your input wiring
	const bool bHasMove = !MoveInputWorld.IsNearlyZero();

	if (bHasMove)
	{
		// Desired linear velocity in world space (units/second)
		const float DesiredSpeed = 1200.0f; // tune per ship
		const FVector DesiredLinearVel = MoveInputWorld.GetSafeNormal() * DesiredSpeed;
		OutProposedMove.LinearVelocity = DesiredLinearVel;
		OutProposedMove.bHasDirIntent = true;
		OutProposedMove.DirectionIntent = MoveInputWorld.GetSafeNormal();
	}
	else
	{
		OutProposedMove.LinearVelocity = FVector::ZeroVector;
		OutProposedMove.bHasDirIntent = false;
	}

	// Angular: use orientation intent (turning) if present
	if (!CharInputs->OrientationIntent.IsNearlyZero())
	{
		// Map orientation intent to an angular velocity rotator (degrees/sec). This is project-specific.
		const FRotator DesiredRot = CharInputs->GetOrientationIntentDir_WorldSpace().Rotation();
		// Simple approach: rotate toward the intent — here we just set a direct angular velocity placeholder
		OutProposedMove.AngularVelocity = FRotator(0.f, DesiredRot.Yaw, 0.f); // yaw-only example
	}
	else
	{
		OutProposedMove.AngularVelocity = FRotator::ZeroRotator;
	}
}

void UChaosPhysicsMoverMode::SimulationTick_Implementation(
	const FSimulationTickParams& Params,
	FMoverTickEndData& OutputState)
{
	// Grab component set
	const FMovingComponentSet& Moving = Params.MovingComps;

	UMoverComponent* MoverComp = Moving.MoverComponent.Get();
	USceneComponent* UpdatedComp = Moving.UpdatedComponent.Get();
	UPrimitiveComponent* UpdatedPrim = Moving.UpdatedPrimitive.Get();

	// We require a primitive with physics simulation enabled
	if (!MoverComp || !UpdatedComp || !UpdatedPrim)
	{
		// Nothing to do, copy start state through
		OutputState.SyncState = Params.StartState.SyncState;
		OutputState.AuxState = Params.StartState.AuxState;
		return;
	}

	if (!UpdatedPrim->IsSimulatingPhysics())
	{
		// If physics not enabled, fallback to kinematic: set actor transform using sweep
		const FVector Delta = Params.ProposedMove.LinearVelocity * (Params.TimeStep.StepMs * 0.001f);
		const FRotator RotDelta = Params.ProposedMove.AngularVelocity * (Params.TimeStep.StepMs * 0.001f);
		FHitResult Hit;
		UpdatedComp->MoveComponent(Delta, (UpdatedComp->GetComponentQuat() * FQuat(RotDelta)).Rotator(), true, &Hit);
	}
	else
	{
		// Physics-driven path: push velocities (preferred simple approach)
		const float DeltaSeconds = Params.TimeStep.StepMs * 0.001f;

		// Proposed move quantities
		const FVector DesiredLinearVel = Params.ProposedMove.LinearVelocity;
		const FRotator DesiredAngularRotVel = Params.ProposedMove.AngularVelocity; // rotator used by Mover API

		// Wake physics
		UpdatedPrim->WakeAllRigidBodies();

		// Option A: directly set physics velocity (authoritative, immediate). Resets existing velocity to desired.
		if (bUseSetVelocityDirectly)
		{
			// Linear
			UpdatedPrim->SetPhysicsLinearVelocity(DesiredLinearVel, false /* bAddToCurrent */, NAME_None);

			// Angular: convert rotator (deg/sec per axis) -> angular velocity vector in degrees/sec (X=Roll, Y=Pitch, Z=Yaw)
			const FVector AngularVelDeg = FVector(DesiredAngularRotVel.Roll, DesiredAngularRotVel.Pitch, DesiredAngularRotVel.Yaw);
			UpdatedPrim->SetPhysicsAngularVelocityInDegrees(AngularVelDeg, false, NAME_None);
		}
		else
		{
			// Option B: apply forces/torques to *accelerate* toward desired velocities (more physically correct)
			const float Mass = UpdatedPrim->GetMass(); // approximate
			const FVector CurrentVel = UpdatedPrim->GetComponentVelocity();
			const FVector VelError = DesiredLinearVel - CurrentVel;
			// simple P controller -> force = kP * velError * mass
			const float kP = 4.0f;
			const FVector ForceToApply = VelError * Mass * kP / FMath::Max(DeltaSeconds, KINDA_SMALL_NUMBER);
			UpdatedPrim->AddForce(ForceToApply, NAME_None, true);

			// Angular: convert and apply torque similarly (approx)
			const FVector CurrentAngVelRad = UpdatedPrim->GetPhysicsAngularVelocityInRadians();
			// Convert desired angular deg/sec to rad/sec for comparison
			const FVector DesiredAngRad = FVector(DesiredAngularRotVel.Roll, DesiredAngularRotVel.Pitch, DesiredAngularRotVel.Yaw) * (PI/180.0f);
			const FVector AngVelError = DesiredAngRad - CurrentAngVelRad;
			const float kPAng = 2.0f;
			const FVector Torque = AngVelError * UpdatedPrim->GetMass() * kPAng / FMath::Max(DeltaSeconds, KINDA_SMALL_NUMBER);
			UpdatedPrim->AddTorqueInRadians(Torque, NAME_None, true);
		}
	}

	// --- Capture the authoritative physics state and write back to Mover sync state ---

	// Mirroring pattern from stock modes: find or add the default sync state struct and set transforms
	FMoverDefaultSyncState& OutputSyncState =
		OutputState.SyncState.SyncStateCollection.FindOrAddMutableDataByType<FMoverDefaultSyncState>();

	// Pull transform/velocity from the primitive (or component root)
	const FVector PhysLocation = UpdatedPrim->GetComponentLocation();
	const FQuat PhysQuat = UpdatedPrim->GetComponentQuat();
	const FVector PhysLinearVel = UpdatedPrim->GetComponentVelocity();
	const FVector PhysAngVelRad = UpdatedPrim->GetPhysicsAngularVelocityInRadians();

	// FMoverDefaultSyncState exposes helper SetTransforms_WorldSpace(...) in the mover API used by WalkingMode etc.
	// SetTransforms_WorldSpace(Location, Orientation, Velocity, MovementBase, BoneName)
	OutputSyncState.SetTransforms_WorldSpace(PhysLocation, PhysQuat.Rotator(), PhysLinearVel, nullptr);

	// If you want to store angular velocity on your sync struct, find that type and set it; otherwise Mover may store it elsewhere.
	// Some Mover setups may expect OutputState.AuxState or a specific struct - adapt as needed.

	// Carry through aux & other sync fields (conservative default: copy start state's other fields)
	OutputState.AuxState = Params.StartState.AuxState;
	// MovementEndState default (no mode switch)
	OutputState.MovementEndState.ResetToDefaults();
}
