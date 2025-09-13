#pragma once

// this was implemented with the help of
// https://www.youtube.com/watch?v=6YffN09Rr_M

#include "CoreMinimal.h"
#include "NetworkPhysicsObjectStructs.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "PhysNetComp.generated.h"

UCLASS(ClassGroup=(Movement), meta=(BlueprintSpawnableComponent))
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
	virtual void InitializeNetworkPhysicsMovement();
	virtual void SimulatePhysicsTick(double DeltaTime, Chaos::FRigidBodyHandle_Internal* RigidBodyHandle);

	// idk why we need these
	class USkeletalMeshComponent* GetSkeletalmeshComponent() const
	{
		return Cast<USkeletalMeshComponent>(GetPawnOwner()->GetRootComponent()); 
	};
	class UStaticMeshComponent* GetStaticMeshComponent() const
	{
		return Cast<UStaticMeshComponent>(GetPawnOwner()->GetRootComponent());
	}
	class UMeshComponent* GetMeshComponent() const
	{
		return Cast<UMeshComponent>(GetPawnOwner()->GetRootComponent());
	}

	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override;
	FBodyInstance* GetBodyInstance() const;
	
	UPROPERTY()
	FObjectSimulationInputs SimulationInputs;
	UPROPERTY()
	FObjectSimulationState SimulationState;

	UFUNCTION(BlueprintCallable)
	void SetForwardInput(float value);
	UFUNCTION(BlueprintCallable)
	void SetRightInput(float value);
	UFUNCTION(BlueprintCallable)
	void SetUpInput(float value);

private:
	UPROPERTY()
	TObjectPtr<UNetworkPhysicsComponent> NetworkPhysicsComponent = nullptr;
	Chaos::FRigidBodyHandle_Internal* RigidHandle = nullptr;
	bool bUsingNetworkPhysicsPrediction = false;
	FBodyInstance* BodyInstance = nullptr;
};

