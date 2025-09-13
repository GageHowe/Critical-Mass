#include "MinimalPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components.h"
#include "Camera/CameraComponent.h"
// #include "movementTypes/HoverMode.h"

AMinimalPawn::AMinimalPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	// RootComponent = Root;

	// MoverComp = CreateDefaultSubobject<UMoverComponent>(TEXT("MoverComponent"));
}

void AMinimalPawn::BeginPlay()
{
	Super::BeginPlay();
}

void AMinimalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMinimalPawn::AsyncPhysicsTickActor(float DeltaTime, float SimTime)
{
	Super::AsyncPhysicsTickActor(DeltaTime, SimTime);
}

void AMinimalPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
