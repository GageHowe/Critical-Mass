#include "MinimalPawn.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components.h"
#include "Camera/CameraComponent.h"
#include "movementTypes/HoverMode.h"

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

// if (MoverComp)
 //        {
 //            UHoverMode* HoverMode = NewObject<UHoverMode>(MoverComp);
 //            MoverComp->AddMovementMode(TEXT("Hover"), HoverMode);
 //    
 //            // activate it
 //            MoverComp->QueueNextMode(TEXT("Hover"));
 //        }
}

void AMinimalPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// void AMinimalPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
// {
// 	Super::SetupPlayerInputComponent(PlayerInputComponent);
// }
