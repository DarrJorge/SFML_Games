project(ZombieArena)

file(GLOB_RECURSE SourceFiles CONFIGURE_DEPENDS 
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
)

add_executable(${PROJECT_NAME} ${SourceFiles}
        src/core/SpawnConfig.cpp
        src/core/SpawnConfig.h
        src/systems/factory/EnemyFactory.cpp
        src/systems/factory/EnemyFactory.h
        src/systems/factory/PickupFactory.cpp
        src/systems/factory/PickupFactory.h)

# loading general lib
target_link_libraries(${PROJECT_NAME} PRIVATE CommonLib)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER "apps")