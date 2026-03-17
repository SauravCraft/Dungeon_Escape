// Fill out your copyright notice in the Description page of Project Settings.

#include "Mover.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMover::UMover()
{
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UMover::BeginPlay()
{
	Super::BeginPlay();

	StartingLocation = GetOwner()->GetActorLocation();
	StartingRotation = GetOwner()->GetActorRotation();

	SetShouldMove(false); // Door starts closed
}

// Called every frame
void UMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ================== LOCATION ==================
	FVector CurrentLocation = GetOwner()->GetActorLocation();

	float Speed = MoveOffset.Length() / TimeMoved;

	FVector TargetLoc = ShouldMove ? (StartingLocation + MoveOffset) : StartingLocation;

	FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLoc, DeltaTime, Speed);

	GetOwner()->SetActorLocation(NewLocation);


	// ================== ROTATION ==================
	FRotator CurrentRotation = GetOwner()->GetActorRotation();

	FRotator TargetRotation;

	if (ShouldMove)
	{
		TargetRotation = StartingRotation + RotateOffset; // Open
	}
	else
	{
		TargetRotation = StartingRotation; // Close
	}

	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 2.0f);

	GetOwner()->SetActorRotation(NewRotation);
}


// ================== GETTER ==================
bool UMover::GetShouldMove()
{
	return ShouldMove;
}


// ================== SETTER ==================
void UMover::SetShouldMove(bool NewShouldMove)
{
	ShouldMove = NewShouldMove;
}