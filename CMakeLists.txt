cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(worminator)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "Worminator")
set(VITA_TITLEID  "MEIR00004")
set(VITA_VERSION  "01.00")


if (BUILD_TYPE MATCHES Release)

   # Build release version with -O3 flag. It will optimize the code and give a good performance boost. 
   message("-- Build mode is RELEASE")
    set(WFLAGS "-W -Wall -Wno-write-strings -Wno-unused -Wno-multichar -Wno-deprecated-declarations -Wno-char-subscripts -Wno-absolute-value -Wno-declaration-after-statement -Wno-misleading-indentation -Wno-implicit-fallthrough -Wno-restrict") 
   
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WFLAGS} -O3 -mcpu=cortex-a9 -mfpu=neon-fp16 -fno-optimize-sibling-calls -ftree-vectorize -fsingle-precision-constant -mtune=cortex-a9 -mfloat-abi=hard -ffast-math -fsigned-char")
   
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WFLAGS} -O3 -mcpu=cortex-a9 -mfpu=neon-fp16 -fno-optimize-sibling-calls -ftree-vectorize -fsingle-precision-constant -mtune=cortex-a9 -mfloat-abi=hard -ffast-math -fpermissive")
   set(ALLEGRO_LIB alleg)
else ()
   # Build with -g flag to enable better debugging information.
   message("-- Build mode is DEBUG")
  set(WFLAGS "-W -Wall -Wno-unused -Wno-multichar -Wno-deprecated-declarations -Wno-char-subscripts -Wno-absolute-value -Wno-declaration-after-statement -Wno-misleading-indentation -Wno-implicit-fallthrough") 
   set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${WFLAGS} -g -std=c99 -fsigned-char")
   set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${WFLAGS} -g -std=c++11")
   set(ALLEGRO_LIB alleg-debug)
   add_definitions(-DPSV_DEBUG_CODE)
endif (BUILD_TYPE MATCHES Release)

add_definitions(-DPSVITA)
add_definitions(-DALLEGRO_PSV)

#Uncomment to use local main
#add_definitions(-DUSE_CONSOLE)


# Add any additional include paths here
include_directories(
  src/
  src/vita_port/
)

add_executable(${PROJECT_NAME}
  src/vita_port/psvita.c
  src/vita_port/ime_dialog.c
  src/worminator.c
)

target_link_libraries(${PROJECT_NAME} ${ALLEGRO_LIB}
  vita2d
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  SceSysmodule_stub
  SceGxm_stub
  SceCommonDialog_stub
  SceHid_stub
  SceAppMgr_stub
  SceAppUtil_stub
  ScePower_stub
  vorbisfile
  vorbis
  mathneon
  modplug
  mikmod
  pthread
  ogg
  jpeg
  png
  z
  m)


vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/pic0.png sce_sys/pic0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml 
)

