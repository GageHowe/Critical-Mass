#pragma once
#include "CoreMinimal.h"
#include "Physics/NetworkPhysicsComponent.h"
#include "PhysicsProxy/SingleParticlePhysicsProxy.h"
#include "NetworkPhysicsObjectStructs.generated.h"

class UPhysNetComp;

// FObjectSimulationInputs

USTRUCT(BlueprintType)
struct FObjectSimulationInputs
{
	GENERATED_BODY()
	// FObjectSimulationInputs() : // constructor with args?
	FObjectSimulationInputs(){};
	~FObjectSimulationInputs(){};
	
	UPROPERTY() float forward = 0;
	UPROPERTY() float right = 0;
	UPROPERTY() float up = 0;

	// this feels like bad practice but it's what the video said
	void resetInputs()
	{
		forward = 0;
		right = 0;
		up = 0;
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