// Fill out your copyright notice in the Description page of Project Settings.


#include "Lock.h"

// Sets default values
ALock::ALock()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Tags.Add("LockItem");

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	TriggerComp = CreateDefaultSubobject<UMyBoxComponent>(TEXT("Trigger Comp"));
	TriggerComp->SetupAttachment(RootComp);

	KeyItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Key Item Mesh"));
	KeyItemMesh->SetupAttachment(RootComp);
}

// Called when the game starts or when spawned
void ALock::BeginPlay()
{
	Super::BeginPlay();
	SetIsKeyPlaced(false);
	
}

// Called every frame
void ALock::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ALock::GetIsKeyPlaced()
{
	return IsKeyPlaced;
}

void ALock::SetIsKeyPlaced(bool KeyPlaced)
{
	IsKeyPlaced = KeyPlaced;
	TriggerComp->Trigger(KeyPlaced);
	KeyItemMesh->SetVisibility(KeyPlaced);
}

