cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 20)

# Configure Vcpkg
find_program(VCPKG_EXECUTABLE vcpkg)#find in PATH a vcpkg variable
get_filename_component(VCPKG_ROOT ${VCPKG_EXECUTABLE} DIRECTORY)
set(CMAKE_TOOLCHAIN_FILE "${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake")
include(${CMAKE_TOOLCHAIN_FILE})

project(Pong CXX) # project definition + check and initialization of all settings and compilateur suffs

find_package(SFML CONFIG REQUIRED COMPONENTS graphics audio system window)

file(GLOB_RECURSE SOURCE_FILES ${PROJECT_SOURCE_DIR}/Pong/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${PROJECT_SOURCE_DIR}/Pong/*.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES} "Pong/src/Game.cpp" "Pong/includes/Game.h")

target_link_libraries(${PROJECT_NAME} sfml-audio sfml-graphics sfml-system sfml-window)