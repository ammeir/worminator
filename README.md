### Port of Worminator 3 for PlayStation Vita.  
Worminator is a sidescrolling action platformer game where you play as The Worminator and fight your way through many levels of madness and mayhem. It features huge levels, two episodes, nine unique weapons, Matrix-style "bullet time", hidden locations, multiple difficulties, several other optional characters and many cheats that influence gameplay.


### Running the game
-Unzip `worminator.zip` and copy the contents to `ux0:data/` so that you have a folder `ux0:data/worminator/data`  
-Install `worminator.vpk`

### Controls
Left analog stick = move player  
Dpad = move player  
Square = shoot  
Cross = jump  
Circle =  duck  
L = bullet time  
R = change weapon  
Triangle + L = statistics  
Triangle + R = change ammo  
Select = main menu  
Start = map  


### Compiling the game 
-Install [VitaSDK](http://vitasdk.org) toolchain  
-If you are on Windows, install [MSYS2](http://msys2.org) command shell  
-Clone worminator repo into a folder somewhere  
-Build and install dependency library [allegro_vita](https://github.com/ammeir/allegro_vita)  
-Compile:  
  cmake "your worminator repo folder" -DBUILD_TYPE=Release  
  make

For a debug version replace Release with Debug.

### Known issues
Game freezes sometimes when player dies riding a tank. 

### Credits
Thanks to David Layne and the rest of the Worminator Team for making this game, releasing the source and making it freeware.  
Thanks to bosshunter (@bhunterzor) for telling me about the game and for beta testing.

