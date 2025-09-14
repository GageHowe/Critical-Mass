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
		
		const FVector Forward = GetOwner()->GetActorForwardVector();
		const FVector Force = Forward * SimulationInputs.forward * 500000.f;

		RigidBodyHandle->AddForce(Force);

		Super::SimulatePhysicsTick(DeltaTime, RigidBodyHandle);
	}
};
