// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoxComponent.h"

UMyBoxComponent::UMyBoxComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMyBoxComponent::BeginPlay()
{
	Super::BeginPlay();

	if (MoverActor) {
		Mover = MoverActor->FindComponentByClass<UMover>();
		if (Mover) {
			UE_LOG(LogTemp, Display, TEXT("Mover Component is Available"));
		}
		else {
			UE_LOG(LogTemp, Display, TEXT("Mover Component is nullptr"));
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("Mover Actor is nullptr"));
	}
	if (IsPressurePlate) {
		OnComponentBeginOverlap.AddDynamic(this, &UMyBoxComponent::OnOverlapBegin);
		OnComponentEndOverlap.AddDynamic(this, &UMyBoxComponent::OnOverlapEnd);
	}

}

void UMyBoxComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMyBoxComponent::Trigger(bool NewTriggerValue)
{
	IsTriggered = NewTriggerValue;
	if (Mover) {
		Mover->SetShouldMove(IsTriggered);
	}
}

void UMyBoxComponent::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator")) {

		ActivatorCount++;
		if (!IsTriggered) {
			Trigger(true);
			
		}
	}
}

void UMyBoxComponent::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor && OtherActor->ActorHasTag("PressurePlateActivator")) {

		ActivatorCount--;
		if (IsTriggered && !ActivatorCount) {
			Trigger(false);
			
		}
	}
}
