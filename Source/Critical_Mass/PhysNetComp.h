#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "PhysNetComp.generated.h"

UCLASS()
class CROSSPAWN_API UPhysNetComp : public UPawnMovementComponent
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;
	virtual void OnCreatePhysicsState() override;
	virtual void OnDestroyPhysicsState() override;
	virtual bool ShouldCreatePhysicsState() const override;

	// OnCreatePhysicsState hooks
	virtual void InitializeNetworkPhysicsMovement(){};
	virtual void SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* RigidHandle){};

	// idk why we need these
	class USkeletalmeshComponent* GetSkeletalmeshComponent();
	class UStaticMeshComponent* GetStaticMeshComponent();
	class UMeshComponent* GetMeshComponent();

	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;
	

private:
	UPROPERTY()
	TObjectPtr<UNetworkPhysicsComponent> NetworkPhysicsComponent = nullptr;
	Chaos::FRigidBodyHandle_Internal* RigidHandle = nullptr;
	bool bUsingNetworkPhysicsPrediction = false;
	
};

