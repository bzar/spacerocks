CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(spacerocks)

find_package(SDL REQUIRED)
find_package(SDL_mixer REQUIRED)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
  ${engineworks_SOURCE_DIR}/include
  ${SDL_INCLUDE_DIR}
)

file(GLOB SPACEROCKS_SOURCES src/*.cpp src/ew/*.cpp src/util/*.cpp)
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++0x)
add_executable(spacerocks ${SPACEROCKS_SOURCES})

target_link_libraries(spacerocks engineworks glfw glhck ${GLFW_LIBRARIES} ${SDL_LIBRARY} ${SDLMIXER_LIBRARY})

install(TARGETS spacerocks DESTINATION .)
install(DIRECTORY img DESTINATION .)
install(DIRECTORY snd DESTINATION .)
install(DIRECTORY fonts DESTINATION .)
file(COPY img DESTINATION .)
file(COPY fonts DESTINATION .)
