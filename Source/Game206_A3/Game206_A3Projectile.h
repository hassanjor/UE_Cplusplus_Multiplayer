// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Game206_A3Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config = Game)
class AGame206_A3Projectile : public AActor
{
	GENERATED_BODY()

		/** Sphere collision component */
		UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
		UProjectileMovementComponent* ProjectileMovement;

public:
	AGame206_A3Projectile();


	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }
};