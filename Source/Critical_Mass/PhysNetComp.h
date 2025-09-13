#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "PhysNetComp.generated.h"

UCLASS()
class CRITICAL_MASS_API UPhysNetComp : public UPawnMovementComponent
{
	GENERATED_BODY()
public:

	UPhysNetComp(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void OnCreatePhysicsState() override;
	virtual void OnDestroyPhysicsState() override;
	virtual bool ShouldCreatePhysicsState() const override;

	// OnCreatePhysicsState hooks
	virtual void InitializeNetworkPhysicsMovement(){};
	virtual void SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* RigidBodyHandle){};

	// idk why we need these
	class USkeletalMeshComponent* GetSkeletalmeshComponent() const;
	class UStaticMeshComponent* GetStaticMeshComponent() const;
	class UMeshComponent* GetMeshComponent() const;

	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;
	FBodyInstance* GetBodyInstance() const;

private:
	UPROPERTY()
	TObjectPtr<UNetworkPhysicsComponent> NetworkPhysicsComponent = nullptr;
	Chaos::FRigidBodyHandle_Internal* RigidHandle = nullptr;
	bool bUsingNetworkPhysicsPrediction = false;
	FBodyInstance* BodyInstance = nullptr;
};

