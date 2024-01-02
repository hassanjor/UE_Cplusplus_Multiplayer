# UE_Cplusplus_Multiplayer
This is a very simple shooter game that showcases multiplayer in unreal engine C++, made by me only. whoever destroys the most targets wins


For this project to work properly make sure you are playing as 2 players and as listen server and the game should open 2 seperate windows one called the "Server" and other "Client" 

Gameplay

- There are 2 guns on the floor and 2 players 

- the 2 players can shoot once they pick up the gun,  and can see each other bullets, and can see each other moving with only the hand mesh, and can see each other guns, and finally can see each others targets that they destroyed

- The 2 players are labeled as (Server) and (Client)

- The Server is Player 1 and Client is player 2, so when it says "Player 1 won" for an example, 
That would mean that the player controlling the server window has won. 

- The score is displayed for both characters inidvidually at the top left of the screen using a widget that gets a score variable from my c++ class 

- The spawning of both characters is random and decided by unreal, I have 2 spawners and unreal chooses where to spawn the server and client and it could be different each time



Code

- First I would like to start with the most confusing part that you might find when looking at the code, The "Fire()" Function for the gun is usually located inside the "TP_WeaponComponent" class
However, after tons of trial and error and research I dont believe that you can actually replicate the bullets fired for both the client and server using that class,
SO, I ended up removing the Fire() function entirley from that class and switched it to the "game206_A3Character" class because in UE4 that is how the character fired the gun + it replicates to the server and client 
using the SAME code I had in the component class, but for somereaason it just did not work there. 

- I created the "Target" class, and then I made a blueprint using that class and assigned the mesh and Material to it.
the class is pretty simple and commented well so should be easy to understand

- As you might have noticed there is a gamemode folder in the content folder, Content->Gamemode. Inside that folder there is a gamemode that I made so I can use in the project 
and change the player state to be my newly made custom player state which is the next point (Other than that all the other stuff should be the same as the default gamemode).

- Finally, onto the "CustomPlayerState" class, this is where I calculate the score for each player and decide the winner, from my research this is where personal variables like score should be calculated according to unreal, This class is fairly straight forward but some stuff can get complex however it is Commented very well and I believe there should be no issue there.


Blueprints 
- made score and gameover widgets using blueprints 
- in level blueprint I display those widgets wheneever needed and I change the level 


So Overall the classes you would want to be looking at are 
TP_WeaponComponent, Game206_A3Character, CustomPlayerState, Target
other than those I did not modify any class (except making the projectile delete it self every 1.5 seconds instead of 3 in the projectile class)
