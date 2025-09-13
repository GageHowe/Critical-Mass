// #pragma once
//
// // base/example class for chaos mover - powered pawns
//
// #include "CoreMinimal.h"
// #include "GameFramework/Pawn.h"
// #include "globalStructs.h"
// #include "Physics/NetworkPhysicsComponent.h"
// #include "Chaos/ParticleHandle.h"
// #include "RollingBallMovementComponent.generated.h"
//
// UCLASS(blueprintable, meta = (BlueprintSpawnableComponent))
// class CRITICAL_MASS_API URollingBallMovementComponent : public UPawnMovementComponent
// {
// 	GENERATED_UCLASS_BODY()
// 	
// public:
//
// 	/** Overridden to allow registration with components NOT owned by a Pawn. */
// 	virtual void SetUpdatedComponent(USceneComponent* NewUpdatedComponent) override;
//
// 	/** Return true if it's suitable to create a physics representation of the Ball at this time */
// 	virtual bool ShouldCreatePhysicsState() const override;
//
// 	/** Used to create any physics engine information for this component */
// 	virtual void OnCreatePhysicsState() override;
//
// 	/** Used to shut down and physics engine structure for this component */
// 	virtual void OnDestroyPhysicsState() override;
//
// 	void InitializeBall();
//
// 	virtual void AsyncPhysicsTickComponent(float DeltaTime, float SimTime) override; // gage - added override
//
// 	UFUNCTION(BlueprintCallable)
// 	void SetThrottleInput(float InThrottle);
//
// 	UFUNCTION(BlueprintCallable)
// 	void SetSteeringInput(float InSteering);
//
// 	UFUNCTION(BlueprintCallable)
// 	void SetTravelDirectionInput(FRotator InTravelDirection);
//
// 	UFUNCTION(BlueprintCallable)
// 	void Jump();
//
// public:
//
// 	// Ball inputs
// 	FBallInputs BallInputs;
//
// 	// Ball state
// 	FTransform BallWorldTransform;
// 	FVector BallForwardAxis;
// 	FVector BallRightAxis;
//
// private:
//
// 	UPROPERTY()
// 	TObjectPtr<UNetworkPhysicsComponent> NetworkPhysicsComponent = nullptr;
//
// 	Chaos::FRigidBodyHandle_Internal* RigidHandle;
// 	FBodyInstance* BodyInstance;
// 	int32 PreviousJumpCount = 0;
// };