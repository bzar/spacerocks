CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(spacerocks)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)
find_package(SDL_net REQUIRED)


OPTION(PANDORA "Build with Pandora specific hacks" OFF)
OPTION(VSYNC "Build vsync support" ON)
OPTION(FULLSCREEN "Fullscreen build" OFF)
OPTION(C4A "Compo4All support" OFF)
OPTION(OPENGL_FIXED "Use fixed pipeline for OpenGL" OFF)

IF (PANDORA)
    ADD_DEFINITIONS(-DPANDORA)
ENDIF (PANDORA)

IF (VSYNC)
    ADD_DEFINITIONS(-DVSYNC)
ENDIF (VSYNC)

IF (FULLSCREEN)
    ADD_DEFINITIONS(-DFULLSCREEN)
ENDIF (FULLSCREEN)

IF (C4A)
    ADD_DEFINITIONS(-DC4A_ENABLED)
ENDIF (C4A)

IF (OPENGL_FIXED)
    ADD_DEFINITIONS(-DOPENGL_FIXED)
ENDIF (OPENGL_FIXED)

# Disable GLHCK features
SET(GLHCK_IMPORT_OPENCTM OFF CACHE BOOL "Disable OpenCTM")
SET(GLHCK_IMPORT_MMD OFF CACHE BOOL "Disable MMD")
SET(GLHCK_IMPORT_BMP OFF CACHE BOOL "Disable BMP")
SET(GLHCK_IMPORT_TGA OFF CACHE BOOL "Disable TGA")
SET(GLHCK_IMPORT_JPEG OFF CACHE BOOL "Disable JPEG")

include_directories(
  ${SDL_INCLUDE_DIR}
)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  lib/sparrow3d
)

file(GLOB SPACEROCKS_SOURCES src/*.cpp src/ew/*.cpp src/util/*.cpp)
list(APPEND CMAKE_CXX_FLAGS -std=c++0x)
add_executable(spacerocks ${SPACEROCKS_SOURCES})

target_link_libraries(spacerocks engineworks glfw glhck sparrowNet ${GLHCK_LIBRARIES} ${GLFW_LIBRARIES} ${SDL_LIBRARY} ${SDLMIXER_LIBRARY} ${SDLNET_LIBRARY})

add_executable(gethighscore util/gethighscore.cpp src/highscoremanager.cpp)
target_link_libraries(gethighscore sparrowNet ${SDL_LIBRARY} ${SDLNET_LIBRARY})

install(TARGETS spacerocks gethighscore DESTINATION .)
install(DIRECTORY img DESTINATION .)
install(DIRECTORY snd DESTINATION .)
install(DIRECTORY fonts DESTINATION .)
file(COPY img DESTINATION .)
file(COPY snd DESTINATION .)
file(COPY fonts DESTINATION .)
