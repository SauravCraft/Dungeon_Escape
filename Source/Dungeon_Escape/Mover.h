// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Mover.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEON_ESCAPE_API UMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UMover();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	FVector MoveOffset;

	UPROPERTY(EditAnywhere)
	FRotator RotateOffset = FRotator(0.0f, 90.0f, 0.0f); // Yaw = 90°
	UPROPERTY(EditAnywhere)

	float TimeMoved = 4.0f;
private:
	UPROPERTY(VisibleAnywhere)
	bool ShouldMove = false;
public:
	bool GetShouldMove();
	void SetShouldMove(bool NewShouldMove);
	UPROPERTY(VisibleAnywhere)
	bool ReachedTarget = false;

	FVector StartingLocation;
	FRotator StartingRotation;
	FVector TargetLocation;

};
