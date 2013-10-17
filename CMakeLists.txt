CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(spacerocks)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)


OPTION(PANDORA "Build with Pandora specific hacks" OFF)
OPTION(VSYNC "Build vsync support" ON)
OPTION(FULLSCREEN "Fullscreen build" OFF)

IF (PANDORA)
    ADD_DEFINITIONS(-DPANDORA)
ENDIF (PANDORA)

IF (VSYNC)
    ADD_DEFINITIONS(-DVSYNC)
ENDIF (VSYNC)

IF (FULLSCREEN)
    ADD_DEFINITIONS(-DFULLSCREEN)
ENDIF (FULLSCREEN)

# Disable GLHCK features
SET(GLHCK_IMPORT_OPENCTM OFF CACHE BOOL "Disable OpenCTM")
SET(GLHCK_IMPORT_MMD OFF CACHE BOOL "Disable MMD")
SET(GLHCK_IMPORT_BMP OFF CACHE BOOL "Disable BMP")
SET(GLHCK_IMPORT_TGA OFF CACHE BOOL "Disable TGA")
SET(GLHCK_IMPORT_JPEG OFF CACHE BOOL "Disable JPEG")

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${SDL_INCLUDE_DIR}
)

file(GLOB SPACEROCKS_SOURCES src/*.cpp src/ew/*.cpp src/util/*.cpp)
list(APPEND CMAKE_CXX_FLAGS -std=c++0x)
add_executable(spacerocks ${SPACEROCKS_SOURCES})

target_link_libraries(spacerocks engineworks glfw glhck ${GLHCK_LIBRARIES} ${GLFW_LIBRARIES} ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})

add_executable(gethighscore util/gethighscore.cpp src/highscoremanager.cpp)

install(TARGETS spacerocks gethighscore DESTINATION .)
install(DIRECTORY img DESTINATION .)
install(DIRECTORY snd DESTINATION .)
install(DIRECTORY fonts DESTINATION .)
file(COPY img DESTINATION .)
file(COPY snd DESTINATION .)
file(COPY fonts DESTINATION .)
