// Copyright Epic Games, Inc. All Rights Reserved.

#include "Game206_A3Character.h"
#include "Game206_A3Projectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Game206_A3Projectile.h"

//////////////////////////////////////////////////////////////////////////
// AGame206_A3Character

AGame206_A3Character::AGame206_A3Character()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
		
	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));


	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 12.0f);

	
}

void AGame206_A3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
	
	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

}

//////////////////////////////////////////////////////////////////////////// Input

void AGame206_A3Character::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGame206_A3Character::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGame206_A3Character::Look);

	}
	

}

void AGame206_A3Character::Fire()
{

	//if the character is null return 
	if (this == nullptr || this->GetController() == nullptr)
	{
		return;
	}
		
	// check if character has the rifle
	if (bHasRifle)
	{
		//make sure projectil class is not NULL
		if (ProjectileClass != nullptr)
		{
			//get a reference to the world and make sure its not NULL
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				//cast to the player controller 
				APlayerController* PlayerController = Cast<APlayerController>(Controller);
				//the rotation for the spawned projectile 
				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
				//The location if the spawned projectile 
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = GetActorLocation() + SpawnRotation.RotateVector(MuzzleOffset);
				//Collision handeling 
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
				
				//Check if character does not have authority 
				if (!HasAuthority())
				{
					//on client 
					//call the firing from the server
					Server_Fire(SpawnLocation, SpawnRotation);
				}
				else
				{
					//else (player has authority) just fire normally 
					// Make the player own the fired projectile so the score Target class Hit Function works 
					AGame206_A3Projectile* Projectile = World->SpawnActor<AGame206_A3Projectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
					if(Projectile)
						Projectile->SetOwner(this); 
				}

			}

		}

		// Try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// Try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = GetMesh1P()->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
	}
}

void AGame206_A3Character::Server_Fire_Implementation(FVector Location, FRotator Rotation)
{	
	//When you call Server_Fire(SpawnLocation, SpawnRotation) in the Fire() function, 
	//This is what will actually get called, its just how the server implementation for functions works in UE
	FActorSpawnParameters ActorSpawnParams;
	ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	// Make the player own the fired projectile so the score Target class Hit Function works 
	AGame206_A3Projectile* Projectile = GetWorld()->SpawnActor<AGame206_A3Projectile>(ProjectileClass, Location, Rotation, ActorSpawnParams);
	if (Projectile)
		Projectile->SetOwner(this);
}




void AGame206_A3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AGame206_A3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGame206_A3Character::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGame206_A3Character::GetHasRifle()
{
	return bHasRifle;
}


