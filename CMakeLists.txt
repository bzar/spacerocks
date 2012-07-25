CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
project(spacerocks)

add_subdirectory(lib)

include_directories(
  ${kazmath_SOURCE_DIR}/src
  ${kazmath_SOURCE_DIR}/src/kazmathxx/src
  ${glhck_SOURCE_DIR}/include
  ${GLFW_SOURCE_DIR}/include
)

file(GLOB SPACEROCKS_SOURCES src/*.cpp)
add_definitions(-DGLHCK_KAZMATH_FLOAT -DUSE_SINGLE_PRECISION)
list(APPEND CMAKE_CXX_FLAGS -std=c++0x)
add_executable(spacerocks ${SPACEROCKS_SOURCES})

target_link_libraries(spacerocks glfw glhck ${GLFW_LIBRARIES})

install(TARGETS spacerocks DESTINATION .)
install(DIRECTORY img DESTINATION .)
