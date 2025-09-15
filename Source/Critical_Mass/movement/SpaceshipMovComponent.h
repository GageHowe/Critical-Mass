#pragma once
#include "CoreMinimal.h"
#include "BasePhysNetComp.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "SpaceshipMovComponent.generated.h"

// this is an example of a networked movement component for an arbitrary physics entity.
// Make sure to call Super::SimulatePhysicsTick as that has some housekeeping stuff.

UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
class USpaceshipMovComponent : public UBasePhysNetComp
{
	GENERATED_BODY()
public:
	virtual void SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* RigidBodyHandle) override
	{
		if (!RigidBodyHandle) return;
		
		FVector Force = FVector::ZeroVector;

		if (SimulationInputs.forward != 0)
		{
			FVector Forward = GetOwner()->GetActorForwardVector();
			Force += Forward * SimulationInputs.forward * 500000.f;
		}

		if (SimulationInputs.up != 0)
		{
			FVector Up = GetOwner()->GetActorUpVector();
			Force += Up * SimulationInputs.up * 500000.f;
		}

		if (SimulationInputs.right != 0)
		{
			FVector Right = GetOwner()->GetActorRightVector();
			Force += Right * SimulationInputs.right * 500000.f;
		}
		
		FVector Torque = FVector::ZeroVector;
		
		if (SimulationInputs.yaw != 0.f)
		{
			Torque += GetOwner()->GetActorUpVector() * SimulationInputs.yaw * 200000.f;
		}
		if (SimulationInputs.pitch != 0.f)
		{
			Torque += GetOwner()->GetActorRightVector() * SimulationInputs.pitch * 200000.f;
		}
		if (SimulationInputs.roll != 0.f)
		{
			Torque += GetOwner()->GetActorForwardVector() * SimulationInputs.roll * 200000.f;
		}
		
		RigidBodyHandle->AddForce(Force);
		RigidBodyHandle->AddTorque(Torque);

		Super::SimulatePhysicsTick(DeltaTime, RigidBodyHandle);
	}
};
