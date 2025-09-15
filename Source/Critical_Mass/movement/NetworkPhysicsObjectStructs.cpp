#include "NetworkPhysicsObjectStructs.h"

#include "BasePhysNetComp.h"


// FObjectNetPhysMoveStates

void FObjectNetPhysMoveStates::ApplyData(UActorComponent* NetworkComponent) const
{
	if (UBasePhysNetComp* PNC = Cast<UBasePhysNetComp>(NetworkComponent))
	{
		PNC->SimulationState = MovementStates;
	}
}

void FObjectNetPhysMoveStates::BuildData(const UActorComponent* NetworkComponent)
{
	if (NetworkComponent)
	{
		if (const UBasePhysNetComp* PNC = Cast<const UBasePhysNetComp>(NetworkComponent))
		{
			MovementStates = PNC->SimulationState;
		}
	}
}

void FObjectNetPhysMoveStates::DecayData(float DecayAmount)
{
	// DecayAmount = FMath::Min(DecayAmount * 2, 1.0f); // never used in the tutorial, not going to worry about it for now
}

bool FObjectNetPhysMoveStates::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FNetworkPhysicsData::SerializeFrames(Ar);
	return true;
}

void FObjectNetPhysMoveStates::InterpolateData(const FNetworkPhysicsData& MinData, const FNetworkPhysicsData& MaxData)
{}

void FObjectNetPhysMoveStates::MergeData(const FNetworkPhysicsData& FromData)
{
	InterpolateData(FromData, *this);
}

// FObjectNetPhysMoveInputs

void FObjectNetPhysMoveInputs::ApplyData(UActorComponent* NetworkComponent) const
{
	if (UBasePhysNetComp* PNC = Cast<UBasePhysNetComp>(NetworkComponent))
	{
		PNC->SimulationInputs = MovementInputs;
	}
}

void FObjectNetPhysMoveInputs::BuildData(const UActorComponent* NetworkComponent)
{
	if (NetworkComponent)
	{
		if (const UBasePhysNetComp* PNC = Cast<const UBasePhysNetComp>(NetworkComponent))
		{
			MovementInputs = PNC->SimulationInputs;
		}
	}
}

void FObjectNetPhysMoveInputs::DecayData(float DecayAmount)
{
	DecayAmount = FMath::Min(DecayAmount * 2, 1.0f);
	MovementInputs.forward = FMath::Lerp(MovementInputs.forward, 0.0, DecayAmount);
	MovementInputs.right = FMath::Lerp(MovementInputs.right, 0.0, DecayAmount);
	MovementInputs.up = FMath::Lerp(MovementInputs.up, 0.0, DecayAmount);
	MovementInputs.pitch = FMath::Lerp(MovementInputs.pitch, 0.0, DecayAmount);
	MovementInputs.yaw = FMath::Lerp(MovementInputs.yaw, 0.0, DecayAmount);
	MovementInputs.roll = FMath::Lerp(MovementInputs.roll, 0.0, DecayAmount);
	
	// finish this later if it's even needed
}

bool FObjectNetPhysMoveInputs::NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess)
{
	FNetworkPhysicsData::SerializeFrames(Ar);
	Ar << MovementInputs.forward;
	Ar << MovementInputs.right;
	Ar << MovementInputs.up;
	Ar << MovementInputs.pitch;
	Ar << MovementInputs.yaw;
	Ar << MovementInputs.roll;
	Ar << MovementInputs.lookDirection;
	// Ar << MovementInputs.LocalImpulse; // for local use only
	return true;
}

void FObjectNetPhysMoveInputs::InterpolateData(const FNetworkPhysicsData& MinData, const FNetworkPhysicsData& MaxData)
{
	const FObjectNetPhysMoveInputs& MinInput = static_cast<const FObjectNetPhysMoveInputs&>(MinData);
	const FObjectNetPhysMoveInputs& MaxInput = static_cast<const FObjectNetPhysMoveInputs&>(MaxData);
	const float LerpFactor = MaxInput.LocalFrame == LocalFrame
		? 1.0f / (MaxInput.LocalFrame - MinInput.LocalFrame + 1) // merge from min into max
		: (LocalFrame - MinInput.LocalFrame) / (MaxInput.LocalFrame - MinInput.LocalFrame); // interpolate from min to max

	MovementInputs.forward = FMath::Lerp(MinInput.MovementInputs.forward, MaxInput.MovementInputs.forward, LerpFactor);
	MovementInputs.right = FMath::Lerp(MinInput.MovementInputs.right, MaxInput.MovementInputs.right, LerpFactor);
	MovementInputs.up = FMath::Lerp(MinInput.MovementInputs.up, MaxInput.MovementInputs.up, LerpFactor);
	MovementInputs.pitch = FMath::Lerp(MinInput.MovementInputs.pitch, MaxInput.MovementInputs.pitch, LerpFactor);
	MovementInputs.yaw = FMath::Lerp(MinInput.MovementInputs.yaw, MaxInput.MovementInputs.yaw, LerpFactor);
	MovementInputs.roll = FMath::Lerp(MinInput.MovementInputs.roll, MaxInput.MovementInputs.roll, LerpFactor);
}

void FObjectNetPhysMoveInputs::MergeData(const FNetworkPhysicsData& FromData)
{
	InterpolateData(FromData, *this);
}



