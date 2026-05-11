cmake_minimum_required(VERSION 3.20)
project(Paint_Program)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BUILD_SHARED_LIBS OFF)

include(FetchContent)
FetchContent_Declare(
        SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

add_executable(Paint_Program main.cpp)

# Link the libraries to your actual target
target_link_libraries(Paint_Program PRIVATE sfml-graphics sfml-window sfml-system)

# Update this to "Flappy_Bird" so CMake knows which target gets the definition
target_compile_definitions(Paint_Program PRIVATE SFML_STATIC)