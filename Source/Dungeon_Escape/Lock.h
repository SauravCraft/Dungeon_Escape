// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyBoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Lock.generated.h"

UCLASS()
class DUNGEON_ESCAPE_API ALock : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	FString LockName;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere)
	UMyBoxComponent* TriggerComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* KeyItemMesh;

	UPROPERTY(EditAnywhere)
	FString KeyItemName;

private:
	bool IsKeyPlaced = false;

public:
	bool GetIsKeyPlaced();

	void SetIsKeyPlaced(bool KeyPlaced);




};
