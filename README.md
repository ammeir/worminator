# Worminator
Port of Worminator 3 for PlayStation Vita.  
Worminator is a sidescrolling action game where you play as The Worminator and fight your way through many levels of madness and mayhem.

### Running the game
-Unzip `worminator.zip` and copy the contents to `ux0:data/` so that you have a folder `ux0:data/worminator/data`  
-Install `worminator.vpk`

### Controls
Left analog stick / Dpad = move player  
Square = shoot  
Cross = jump  
Triangle =  statistics  
Circle =  duck  
R = bullet time  
L = change weapon  
Select = main menu  
Start = map  


### Compiling the game 
-Install [VitaSDK](http://vitasdk.org) toolchain  
-If you are on Windows, install [MSYS2](http://msys2.org) command shell  
-Clone worminator repo into a folder somewhere  
-Build and install dependency library allegro_vita  
-Compile:  
  cmake "your worminator repo folder" -DBUILD_TYPE=Release  
  make

For a debug version replace Release with Debug.

### Known issues
Game freezes in level 14 if the player dies riding a tank 

### Credits
Thanks to David Layne and team for making this game, releasing the source and making it freeware.  
Thanks to bosshunter (@bhunterzor) for introducing the game and for beta testing.

