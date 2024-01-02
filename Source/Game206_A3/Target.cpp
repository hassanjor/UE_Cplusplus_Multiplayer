// Fill out your copyright notice in the Description page of Project Settings.


#include "Target.h"
#include "Game206_A3Projectile.h"
#include "Game206_A3GameMode.h"
#include "Game206_A3Character.h"
#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Making a mesh that can be set in the scene editor
	TargetMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	TargetMesh->SetupAttachment(RootComponent);

	
}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	//The OnHit will detect if only the bullet hits the mesh to be more accurate
	TargetMesh->OnComponentHit.AddDynamic(this, &ATarget::TargetHit);

}

void ATarget::TargetHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{

	//If the actor that hits the cube is the projectile
	if (AGame206_A3Projectile* Projectile = Cast<AGame206_A3Projectile>(OtherActor))
	{
		
		// If the player is the owner of the projectile (I make the player the owner inside fire() in character.cpp right when we spawn it.
		if (AGame206_A3Character* PlayerShooting = Cast<AGame206_A3Character>(Projectile->GetOwner()))
		{
			//If the player state is the same as the custom state I made 
			if (ACustomPlayerState* PS = PlayerShooting->GetPlayerState<ACustomPlayerState>())
			{
				//Add 1 to the score 
				PS->ModifyScore(1); // Increase the score when a target is hit
				// Destroy the target and projectile
				Destroy();
				Projectile->Destroy();
					

			}
					
		}

	}

}

