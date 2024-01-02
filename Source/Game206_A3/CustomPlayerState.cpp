// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include <Net/UnrealNetwork.h>


ACustomPlayerState::ACustomPlayerState()
{
	//make sure score starts at 0 
	PlayerScore = 0; 
    //so replicating works
    bReplicates = true;
}

//If I dont replicate the variable sometimes the score UI misses up and forgets about 1 or more targets
//Thats why i do this here 
void ACustomPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
    DOREPLIFETIME(ACustomPlayerState, PlayerScore);
}

void ACustomPlayerState::ModifyScore(int ScoreChange)
{
	//increase score to whatever value given
    PlayerScore += ScoreChange;
    UE_LOG(LogTemp, Warning, TEXT("%d"), PlayerScore); 
    //check the owner of the controller
	if (APlayerController* OwnerController = Cast<APlayerController>(GetOwner()))
	{
        //Print the name of the player that just hit the target 
        
        //THIS HELPS A LOT WHEN YOU ARE PLAYING TO SEE WHO IS PLAYER 1 AND 2
        FString test = OwnerController->GetName(); 
        UE_LOG(LogTemp, Warning, TEXT("%s"), *test);

        // Check if there's another player state (PlayerState for player 2)
        ACustomPlayerState* OtherPlayerState = nullptr;
        for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
        {
            const TWeakObjectPtr<APlayerController>& PlayerController = *Iterator;
            const ACustomPlayerState* PlayerState = Cast<const ACustomPlayerState>(PlayerController->PlayerState);

            if (PlayerState && PlayerState != this)
            {
                OtherPlayerState = const_cast<ACustomPlayerState*>(PlayerState);
                break;
            }
        }
        //If theres another player state
        if (OtherPlayerState)
        {
            //If both player have a combined score of 19 (meaning all targets are desrtoyed)
            if (this->PlayerScore + OtherPlayerState->PlayerScore >= 19)
            {
                //This allows the game to pick a winner 
                AllTargetsDestoryed = true; 
            }
        }

        //If all the targets are destroyed 
        if (AllTargetsDestoryed == true)
        {
           
            //Other player state is valid
             if (OtherPlayerState)
             {
                 
                 //I could not come up with any other way that could make winning and losing work, nothing helped,
                 //after tons of trial and error I came up with the idea of determining the winners by using the specific name of each player

                 //I set the server which is "PlayerController_0" to be my PLAYER 1 
                 // And the client which is "PlayerController_1" to be my PLAYER 2
                  
                 //FYI: The print statments prints depending on who is being controlled at the moment of all the actors being destoyed, 
                 //that is why I make sure to go through both cases for each player so the print of the winner is accurate. 
                 //It basically wont always print if I dont do that because sometimes the player can be controlled and loses if the score margine is big 
                  
                 
                 //This is a direct reference to the "Server" player when you are playing in the scene editor 
                 if (OwnerController->GetName() == "PlayerController_0") 
                 {
                     //if this specific server  player has MORE score than the other player which would be ("PlayerController_1" and is the client) then the server(PLAYER 1) has WON THE GAME
                     if (this->PlayerScore > OtherPlayerState->PlayerScore)
                     {
                         
                         GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("PLAYER 1 (SERVER) WON")), false, FVector2D(3.0f, 3.0f));
                     }

                     //if this specific server player has LESS score than the other player which is ("PlayerController_1" and is the client) then the server has LOST THE GAME,  and PLAYER 2 who is "PlayerController_1" Has WON
                     if (this->PlayerScore < OtherPlayerState->PlayerScore)
                     {
                         
                         GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("PLAYER 2 (CLIENT) WON")), false, FVector2D(3.0f, 3.0f));
                     }

                 }

                 //This is the opposite of the above, 
                 
                 //This is a direct reference to the Client when you are playing in the scene editor 
                 if (OwnerController->GetName() == "PlayerController_1")
                 {
                     //If this specific Client player has MORE score than the other player which is ("PlayerController_0" and is the server) then The Client(Player 2) WON THE GAME
                     if (this->PlayerScore > OtherPlayerState->PlayerScore)
                     {
                         GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("PLAYER 2 (CLIENT) WON")), false, FVector2D(3.0f, 3.0f));
                     }

                     //If this specific Client player has LESS score than the other player which is ("PlayerController_0" and is the server) then The Client(Player 2) LOST THE GAME,  and Player1 who is "PlayerController_0" Has WON
                     if (this->PlayerScore < OtherPlayerState->PlayerScore)
                     {
                         GEngine->AddOnScreenDebugMessage(-1, 20.0f, FColor::Red, FString::Printf(TEXT("PLAYER 1 (SERVER) WON")), false, FVector2D(3.0f, 3.0f));
                     }

                 }
                

             }
              
            
        }
   
    }
}




