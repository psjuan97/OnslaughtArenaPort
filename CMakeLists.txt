cmake_minimum_required(VERSION 3.2)

set(CMAKE_BUILD_TYPE Release)


set(CMAKE_C_FLAGS "-Wall -Wextra -O2")
set(CMAKE_CXX_FLAGS "-Wall -Wextra -O2")


if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})


## Use the variable PROJECT_NAME for changing the target name
set( PROJECT_NAME "OnslaughtArena" )
set( VITA_PROJECT_NAME "Onslaught Arena" )

## Set our project name
project(${PROJECT_NAME})

include("${VITASDK}/share/vita.cmake" REQUIRED)
find_package(SDL2 REQUIRED)


set(VITA_TITLEID  "ONAR00001")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")



include_directories("src")

## Use all the *.cpp files we found under this folder for the project
FILE(GLOB_RECURSE SRCS "src/*.cpp" "src/*.c")

## Define the executable
add_executable(${PROJECT_NAME} ${SRCS})

# Detect and add SDL, Box2D and more
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})




if(TinyXML2_FOUND)
  target_link_libraries(${PROJECT_NAME} ${TinyXML2_LIBRARIES})
endif()

if(BOX2D_FOUND)
  target_link_libraries(${PROJECT_NAME} ${BOX2D_LIBRARY})
endif()

if(SDL2_FOUND)
  include_directories(${SDL2_INCLUDE_DIR})
  target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY})
endif()

if(SDL2_IMAGE_FOUND)
  target_link_libraries(${PROJECT_NAME} ${SDL2_IMAGE_LIBRARIES})
endif()

  target_link_libraries(${PROJECT_NAME} ${SDL2_TTF_LIBRARIES})

target_link_libraries(${PROJECT_NAME}
SDL2_ttf
freetype
png
z
bz2
SDL2_image
vita2d
SceMotion_stub
SceDisplay_stub
SceCtrl_stub
SceAudio_stub
SceAudioIn_stub
SceSysmodule_stub
SceGxm_stub
SceCommonDialog_stub
SceAppMgr_stub
SceTouch_stub
SceHid_stub
m
stdc++
pthread
)



vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_PROJECT_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE js js
  FILE img img
)
