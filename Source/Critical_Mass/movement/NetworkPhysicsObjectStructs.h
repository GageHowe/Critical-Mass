#pragma once
#include "CoreMinimal.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "NetworkPhysicsObjectStructs.generated.h"

class UBasePhysNetComp;

// FObjectSimulationInputs

USTRUCT(BlueprintType)
struct FObjectSimulationInputs
{
	GENERATED_BODY()
	// FObjectSimulationInputs() : // constructor with args?
	FObjectSimulationInputs(){};
	~FObjectSimulationInputs(){};

	// keeping all these is probably ok since Unreal delta compresses
	UPROPERTY() float forward = 0;
	UPROPERTY() float right = 0;
	UPROPERTY() float up = 0;
	UPROPERTY() float yaw = 0;
	UPROPERTY() float pitch = 0;
	UPROPERTY() float roll = 0;
	UPROPERTY() FVector lookDirection = {0,0,0};

	// used to locally predict impulses from explosions like rocket jumps
	UPROPERTY(NotReplicated) FVector LocalImpulse = {0,0,0};

	void resetInputs()
	{
		forward = 0;
		right = 0;
		up = 0;
		yaw = 0;
		pitch = 0;
		roll = 0;
		lookDirection = {0,0,0};
		LocalImpulse = {0,0,0};
	}
};

// FObjectSimulationState

USTRUCT(BlueprintType)
struct FObjectSimulationState
{
	GENERATED_BODY()
};

// FObjectNetPhysMoveStates

USTRUCT(BlueprintType)
struct FObjectNetPhysMoveStates : public FNetworkPhysicsData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FObjectSimulationState MovementStates;

	virtual void ApplyData(UActorComponent* NetworkComponent) const override;
	virtual void BuildData(const UActorComponent* NetworkComponent) override;
	virtual void DecayData(float DecayAmount) override;
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	virtual void InterpolateData(const FNetworkPhysicsData& MinData, const FNetworkPhysicsData& MaxData) override;
	virtual void MergeData(const FNetworkPhysicsData& FromData) override;
};
template<>
struct TStructOpsTypeTraits<FObjectNetPhysMoveStates> : public TStructOpsTypeTraitsBase2<FObjectNetPhysMoveStates>
{
	enum
	{
		WithNetSerializer = true,
	};
};

// FObjectNetPhysMoveInputs

USTRUCT(BlueprintType)
struct FObjectNetPhysMoveInputs : public FNetworkPhysicsData
{
	GENERATED_BODY()
public:
	UPROPERTY()
	FObjectSimulationInputs MovementInputs;

	virtual void ApplyData(UActorComponent* NetworkComponent) const override;
	virtual void BuildData(const UActorComponent* NetworkComponent) override;
	virtual void DecayData(float DecayAmount) override;
	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
	virtual void InterpolateData(const FNetworkPhysicsData& MinData, const FNetworkPhysicsData& MaxData) override;
	virtual void MergeData(const FNetworkPhysicsData& FromData) override;
};

template<>
struct TStructOpsTypeTraits<FObjectNetPhysMoveInputs> : public TStructOpsTypeTraitsBase2<FObjectNetPhysMoveInputs>
{
	enum
	{
		WithNetSerializer = true,
	};
};

struct FObjectPhysicsMovementTraits
{
	using InputsType = FObjectNetPhysMoveInputs;
	using StatesType = FObjectNetPhysMoveStates;
};