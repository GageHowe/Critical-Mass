// #pragma once
//
// #include "CoreMinimal.h"
// #include "GameFramework/Pawn.h"
// #include "NetworkPredictionWorldManager.h"
// #include "NetworkPhysicsPawn.generated.h"
//
// class UStaticMeshComponent;
//
// // Service to connect pawn to NetworkPrediction rollback system
// class FNetworkPhysicsService : public IPhysicsService, public IFixedRollbackService
// {
// public:
// 	FNetworkPhysicsService(class ANetworkPhysicsPawn* InPawn)
// 		: Pawn(InPawn)
// 	{}
//
// 	// Called after resimulated physics frame
// 	virtual void PostResimulate(FNetworkPredictionWorldManager::FNetworkPredictionTickState* FixedTickState) override
// 	{
// 		if (Pawn && Pawn->Mesh)
// 		{
// 			Pawn->Mesh->UpdateComponentToWorld();
// 		}
// 	}
//
// 	// Ensures pawn is in sync with GT (editor only)
// 	virtual void EnsureDataInSync(const TCHAR* Context) override
// 	{
// #if NP_ENSURE_PHYSICS_GT_SYNC
// 		if (Pawn && Pawn->Mesh)
// 		{
// 			FTransform PhysTM = Pawn->Mesh->GetComponentTransform();
// 			Pawn->SetActorTransform(PhysTM);
// 		}
// #endif
// 	}
//
// 	// Called during rollback to apply inputs
// 	virtual void PreStepRollback(FNetSimTimeStep& Step, FServiceTimeStep& ServiceStep, int32 Offset, bool bFirstStep) override {}
// 	virtual void StepRollback(FNetSimTimeStep& Step, FServiceTimeStep& ServiceStep) override
// 	{
// 		if (Pawn)
// 		{
// 			Pawn->ApplyInputToPhysics(Step.StepMS);
// 		}
// 	}
//
// private:
// 	ANetworkPhysicsPawn* Pawn;
// };
//
// UCLASS()
// class ANetworkPhysicsPawn : public APawn
// {
// 	GENERATED_BODY()
// public:
// 	ANetworkPhysicsPawn();
//
// 	virtual void BeginPlay() override;
// 	virtual void Tick(float DeltaTime) override;
//
// 	// Called by FNetworkPhysicsService during rollback
// 	void ApplyInputToPhysics(float DeltaMS);
//
// 	UPROPERTY(VisibleAnywhere)
// 	UStaticMeshComponent* Mesh;
//
// 	// Movement inputs
// 	FVector PendingForce;
//
// 	// NetworkPrediction service
// 	TUniquePtr<FNetworkPhysicsService> PhysicsService;
//
// 	// Simple input interface
// 	void SetMoveInput(FVector Force) { PendingForce = Force; }
// };
