// Copyright Epic Games, Inc. All Rights Reserved.

#include "Dungeon_EscapeCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Dungeon_Escape.h"
#include "CollectableItem.h"
#include "Lock.h"

ADungeon_EscapeCharacter::ADungeon_EscapeCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void ADungeon_EscapeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ADungeon_EscapeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ADungeon_EscapeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ADungeon_EscapeCharacter::LookInput);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &ADungeon_EscapeCharacter::DoInteract);
	}
	else
	{
		UE_LOG(LogDungeon_Escape, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}


void ADungeon_EscapeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void ADungeon_EscapeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void ADungeon_EscapeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ADungeon_EscapeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ADungeon_EscapeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ADungeon_EscapeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void ADungeon_EscapeCharacter::DoInteract()
{

	FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	FVector Direction = FirstPersonCameraComponent->GetForwardVector();
	FVector End = Start + (Direction*MaxInteractionDistance);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 5.0f);
	FCollisionShape InteractSphere = FCollisionShape::MakeSphere(InteractionSphereRadius);
	DrawDebugSphere(GetWorld(), End, InteractionSphereRadius, 40.0f, FColor::Blue, false, 5.0f);

	FHitResult HitResult;
	bool IsInteract = GetWorld()->SweepSingleByChannel(
		HitResult,
		Start,End,
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		InteractSphere);

	AActor* HitActor = HitResult.GetActor();

	if (IsInteract) {
		if (HitActor->ActorHasTag("Collectable")) {
			ACollectableItem* Collectable = Cast<ACollectableItem>(HitActor);
			Inventory.Add(Collectable->ItemName);
			Collectable->Destroy();
		}
		else if (HitActor->ActorHasTag("LockItem")) {
			ALock* LockNameIs = Cast<ALock>(HitActor);

			if (LockNameIs) {
				if (!LockNameIs->GetIsKeyPlaced()) {
					int32 isItem = Inventory.RemoveSingle(LockNameIs->KeyItemName);
					if (isItem) {
						LockNameIs->SetIsKeyPlaced(true);
					}
					else {
						UE_LOG(LogTemp, Display, TEXT("LockItem Not in Inventory"));
					}
				}
				else {
					UE_LOG(LogTemp, Display, TEXT("LockItem Is Present"));
					Inventory.Add(LockNameIs->KeyItemName);
					LockNameIs->SetIsKeyPlaced(false);
					
				}
			}
			
			/*for (int32 i = 0; i < Inventory.Num(); i++) {
				if ( Inventory[i] == LockNameIs->KeyItemName ) {
					if (!LockNameIs->GetIsKeyPlaced()) {
						LockNameIs->SetIsKeyPlaced(true);
						Inventory.Remove(LockNameIs->KeyItemName);
					}
					 
				}
			}*/
			
		}
	}
	else {
		UE_LOG(LogTemp, Display, TEXT("No Actor Detected"));
	}

}

