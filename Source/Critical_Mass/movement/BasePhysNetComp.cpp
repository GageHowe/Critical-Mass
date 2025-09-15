#include "BasePhysNetComp.h"
// #include "NetworkPhysicsObjectStructs.h"
#include "NetworkPhysicsObjectStructs.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

UBasePhysNetComp::UBasePhysNetComp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	SetIsReplicatedByDefault(true);
	bUsingNetworkPhysicsPrediction = Chaos::FPhysicsSolverBase::IsNetworkPhysicsPredictionEnabled();

	// if we aren't playing standalone, set up network stuff
	if (bUsingNetworkPhysicsPrediction)
	{
		static const FName NetworkPhysicsComponentName(TEXT("PC_NetworkPhysicsComponent"));
		NetworkPhysicsComponent = CreateDefaultSubobject<UNetworkPhysicsComponent, UNetworkPhysicsComponent>(NetworkPhysicsComponentName);
		NetworkPhysicsComponent->SetNetAddressable();
		NetworkPhysicsComponent->SetIsReplicated(true);
	}
}

void UBasePhysNetComp::BeginPlay()
{
	Super::BeginPlay();
}

void UBasePhysNetComp::OnCreatePhysicsState()
{
	Super::OnCreatePhysicsState();
	UWorld* World = GetWorld();
	if (World->IsGameWorld())
	{
		if (UpdatedComponent && UpdatedPrimitive)
		{
			SetAsyncPhysicsTickEnabled(true);
		}
		InitializeNetworkPhysicsMovement();
	}

	if (UPrimitiveComponent* Mesh = Cast<UPrimitiveComponent>(UpdatedComponent))
	{
		BodyInstance = Mesh->GetBodyInstance();
	}
}

void UBasePhysNetComp::OnDestroyPhysicsState()
{
	Super::OnDestroyPhysicsState();

	if (UpdatedComponent)
	{
		UpdatedComponent->RecreatePhysicsState();
	}

	if (NetworkPhysicsComponent)
	{
		NetworkPhysicsComponent->RemoveDataHistory();
	}
}

bool UBasePhysNetComp::ShouldCreatePhysicsState() const
{
	// return Super::ShouldCreatePhysicsState();

	if (!IsRegistered() || IsBeingDestroyed())
	{
		return false;
	}

	UWorld* World = GetWorld();
	if (World->IsGameWorld())
	{
		FPhysScene* PhysScene = World->GetPhysicsScene();
		if (PhysScene && UpdatedPrimitive)
		{
			return true;
		}
	}
	return false;
}

void UBasePhysNetComp::InitializeNetworkPhysicsMovement()
{
	if (bUsingNetworkPhysicsPrediction)
	{
		if (NetworkPhysicsComponent)
		{
			NetworkPhysicsComponent->CreateDataHistory<FObjectPhysicsMovementTraits>(this);
		}
	}
}

// I guess we override this in children for other vehicles etc
void UBasePhysNetComp::SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* RigidBodyHandle)
{
	if (!RigidBodyHandle) return;

	// // if client and has unexpected force, apply it
	// if (!GetOwner()->HasAuthority() && !SimulationInputs.LocalImpulse.IsNearlyZero())
	// {
	// 	RigidBodyHandle->AddForce(SimulationInputs.LocalImpulse);
	// }
	
	SimulationInputs.resetInputs();
}

void UBasePhysNetComp::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	UNavMovementComponent::SetUpdatedComponent(NewUpdatedComponent);
	PawnOwner = NewUpdatedComponent ? Cast<APawn>(NewUpdatedComponent->GetOwner()) : nullptr;
}

void UBasePhysNetComp::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickComponent(DeltaTime, SimTime);

	if (!BodyInstance) return;

	if (const FPhysicsActorHandle Handle = BodyInstance->ActorHandle)
	{
		RigidHandle = Handle->GetPhysicsThreadAPI();
	}

	UWorld* World = GetWorld();
	if (World && RigidHandle)
	{
		SimulatePhysicsTick(DeltaTime, RigidHandle);
	}
}

FBodyInstance* UBasePhysNetComp::GetBodyInstance() const
{
	return BodyInstance;
}

void UBasePhysNetComp::SetForwardInput(float value)
{
	SimulationInputs.forward = value;
}

void UBasePhysNetComp::SetRightInput(float value)
{
	SimulationInputs.right = value;
}

void UBasePhysNetComp::SetUpInput(float value)
{
	SimulationInputs.up = value;
}

void UBasePhysNetComp::SetPitchInput(float value)
{
	SimulationInputs.pitch = value;
}

void UBasePhysNetComp::SetYawInput(float value)
{
	SimulationInputs.yaw = value;
}

void UBasePhysNetComp::SetRollInput(float value)
{
	SimulationInputs.roll = value;
}

