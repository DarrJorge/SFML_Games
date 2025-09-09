cmake_minimum_required(VERSION 3.15)

# setup project
set(PROJECT_NAME SFMLPractice)
project(${PROJECT_NAME} VERSION 1.0.0)
set(STARTUP_PROJECT_NAME Timber)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

message("------------Project files generation for ${PROJECT_NAME}-----")

# common options
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# (Optional) group targets neatly in IDE
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# find libs
include(FetchContent)

# Disable examples/tests SFML
set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_TESTS    OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    SFML
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG "3.0.1"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(SFML)

# (Optional) Common compilation options: connect as INTERFACE target
add_library(CommonLib INTERFACE Source/ResourceManager.h)
add_library(ResourceManagerLib INTERFACE)

target_include_directories(ResourceManagerLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/Source)

target_compile_features(CommonLib INTERFACE cxx_std_20)

target_link_libraries(CommonLib INTERFACE SFML::Graphics SFML::Audio)
target_link_libraries(CommonLib INTERFACE ResourceManagerLib)

# === Subprojects ===
add_subdirectory(Source/Timber)
add_subdirectory(Source/ZombieArena)

# For Visual Studio specify the startup project (CLion ignores this,
# but it doesn’t hurt; in CLion the startup target is chosen in Run/Debug configurations)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${STARTUP_PROJECT_NAME})
