// #include "NetworkPhysicsPawn.h"
// #include "Components/StaticMeshComponent.h"
// #include "Engine/World.h"
// #include "Chaos/ChaosSolverActor.h"
// #include "Chaos/ChaosEngineInterface.h"
// #include "NetworkPredictionWorldManager.h"
// #include "Chaos/ChaosSolver.h"
//
// ANetworkPhysicsPawn::ANetworkPhysicsPawn()
// {
// 	PrimaryActorTick.bCanEverTick = true;
//
// 	// Create physics mesh
// 	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
// 	RootComponent = Mesh;
// 	Mesh->SetSimulatePhysics(true);
// 	Mesh->SetEnableGravity(true);
// 	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));
// 	Mesh->SetMobility(EComponentMobility::Movable);
//
// 	PendingForce = FVector::ZeroVector;
// }
//
// void ANetworkPhysicsPawn::BeginPlay()
// {
// 	Super::BeginPlay();
//
// 	// Enable physics rollback
// 	UNetworkPredictionWorldManager* Manager = UNetworkPredictionWorldManager::Get();
// 	if (Manager)
// 	{
// 		Manager->SetUsingPhysics();
// 		Manager->InitPhysicsCapture();
//
// 		PhysicsService = MakeUnique<FNetworkPhysicsService>(this);
// 		Manager->Services.FixedPhysics.Add(MoveTemp(PhysicsService));
// 		Manager->Services.FixedRollback.Add(MoveTemp(PhysicsService));
// 	}
// }
//
// void ANetworkPhysicsPawn::Tick(float DeltaTime)
// {
// 	Super::Tick(DeltaTime);
//
// 	// Only apply forces locally (real-time)
// 	if (PendingForce.SizeSquared() > 0.f)
// 	{
// 		Mesh->AddForce(PendingForce, NAME_None, true);
// 		PendingForce = FVector::ZeroVector;
// 	}
// }
//
// // Called by service during rollback
// void ANetworkPhysicsPawn::ApplyInputToPhysics(float DeltaMS)
// {
// 	if (PendingForce.SizeSquared() > 0.f && Mesh)
// 	{
// 		const float Scale = DeltaMS / 1000.f; // Convert ms to seconds
// 		Mesh->AddForce(PendingForce * Scale, NAME_None, true);
// 	}
// }
