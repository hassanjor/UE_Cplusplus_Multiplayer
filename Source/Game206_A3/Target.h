// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"


UCLASS()
class GAME206_A3_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//The mesh
	UPROPERTY(VisibleAnywhere)
		class UStaticMeshComponent* TargetMesh;

	//Checking when the target was hit 
	UFUNCTION(BlueprintCallable)
		void TargetHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

};
