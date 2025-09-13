#include "PhysNetComp.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

UPhysNetComp::UPhysNetComp(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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

void UPhysNetComp::BeginPlay()
{
	Super::BeginPlay();
}

void UPhysNetComp::OnCreatePhysicsState()
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

void UPhysNetComp::OnDestroyPhysicsState()
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

bool UPhysNetComp::ShouldCreatePhysicsState() const
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

class USkeletalMeshComponent* UPhysNetComp::GetSkeletalmeshComponent() const
{
	return Cast<USkeletalMeshComponent>(GetPawnOwner()->GetRootComponent());
}

class UStaticMeshComponent* UPhysNetComp::GetStaticMeshComponent() const
{
	return Cast<UStaticMeshComponent>(GetPawnOwner()->GetRootComponent());
}

class UMeshComponent* UPhysNetComp::GetMeshComponent() const
{
	return Cast<UMeshComponent>(GetPawnOwner()->GetRootComponent());
}

void UPhysNetComp::SetUpdatedComponent(USceneComponent* NewUpdatedComponent)
{
	// Super::SetUpdatedComponent(NewUpdatedComponent);
	UNavMovementComponent::SetUpdatedComponent(NewUpdatedComponent);
	PawnOwner = NewUpdatedComponent ? Cast<APawn>(NewUpdatedComponent->GetOwner()) : nullptr;
}

void UPhysNetComp::AsyncPhysicsTickComponent(float DeltaTime, float SimTime)
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
		// deferredforcesloader.Apply(RigidHandle) // not using this probably
	}
}

FBodyInstance* UPhysNetComp::GetBodyInstance() const
{
	return BodyInstance;
}

