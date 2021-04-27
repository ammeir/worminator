
HumbleSpeccy is called ZXVita on the PSVita port.

How to compile zxvita.vpk:

1. Run msys64\mingw64.exe (this is a unix-like command shell window where we compile our code)
2. cd /usr/local/zxvita/ver/1xx (1xx means current development version)
3. make


If this is a first build and makefile does not exist:

1. create CMakeLists.txt for your project. Look at one of the samples projects for example.
2. mkdir build
3. cd build
4. cmake .. (creates the makefile)
5. make


If you get the following error message:
CMake Error at CMakeLists.txt:11 (message): Please define VITASDK to point to your SDK path!

Run export VITASDK=/usr/local/vitasdk



How to install vpk:

1. Run Henkaku on vita
2. Open VitaShell
3. Connect vita to pc with usb cable 
4. Press Select-button on vita to activate usb
5. Copy zxvita.vpk file to ux0:vpk (ux0 is memory card)
6. Run zxvita.vpk (this will install and create a bubble on the desktop)

Note:
All images in sce_sys folder need to be in 8-bit format. Otherwise the vpk won't install (Error 0x8010113D). Just run pngquant on all your png images (d:\home\tools\pngquant-windows).


Visual studio project:
\msys64\usr\local\zxvita\ver\1xx\HSVita.sln
Edit CMakeLists.txt when adding new classes to the project.


Update VitaSDK:
1. Open mingw64.exe
2. cd /home/ammeir/vpdm
3. ./vitasdk-update
If you get error $VITASDK not set, run: export VITASDK=/usr/local/vitasdk

Update DolceSDK:
1. Open mingw64.exe
2. cd /home/ammeir/ddpm
3. python dolcesdk-update.py
If you get error $DOLCESDK not set, run: 
export DOLCESDK=/usr/local/dolcesdk
export PATH="$DOLCESDK/bin:$PATH"

This will update the toolchain and the most common libraries


How to read core dumps:
1. Open mingw32 command window (this won't work on mingw64 window)
2. Make sure /usr/local/vitasdk/bin path is in the PATH environmental variable
   env-command prints all environmental variables.
   to add the path run: export PATH=$PATH:/usr/local/vitasdk/bin
3. Copy .psp2dump and .elf files to  /usr/local/coredumps
4. Run vitaparsecoredump.sh <.psp2dump> <.elf>
   .psp2dump is the file the vita produces whem program crashes
   .elf is the file make produces. It is the file without extension name
   Example: vitaparsecoredump.sh psp2core-1557615991-0x0000092a6d-eboot.bin.psp2dmp zxvita



Development Notes:
-Failed to use sdl2 to draw the screen. Did not find a way to scale the emulator screen to vita's whole screen.
-Changed to vita2d library and it scales the picture fine.
-Created a project 'humble_speccy_merge' to debug my cpu. It runs a program using two cpu's and two register sets. 
After each instruction the two register sets are compared for differences.
The second cpu is supposed to work correctly. The author wrote that it passes the zexall instruction test.

