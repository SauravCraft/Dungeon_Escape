// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Mover.h"
#include "MyBoxComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup = (Custom), meta = (BlueprintSpawnableComponent) )
class DUNGEON_ESCAPE_API UMyBoxComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
	UMyBoxComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere)
	AActor* MoverActor;


	UPROPERTY(EditAnywhere)
	bool IsPressurePlate = false;

	UPROPERTY(VisibleAnyWhere)
	bool IsTriggered = false;
	UPROPERTY(VisibleAnyWhere)
	int32 ActivatorCount = 0;

	void Trigger(bool NewTriggerValue);

	UMover* Mover;


	UFUNCTION()
	void OnOverlapBegin( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd( UPrimitiveComponent* OverlappedComp,  AActor* OtherActor,  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
