// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CustomPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class GAME206_A3_API ACustomPlayerState : public APlayerState
{
	GENERATED_BODY()
private:
	ACustomPlayerState(); 
public:
	//replicate variable
	UPROPERTY(replicated, BlueprintReadOnly , EditAnywhere)
	int PlayerScore;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool AllTargetsDestoryed = false;

	UFUNCTION()
	void ModifyScore(int ScoreChange); 


};

