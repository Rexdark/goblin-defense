cmake_minimum_required(VERSION 3.23)
project(GoblinDefense LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)

# Download and add SFML as a dependency
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Download and add tmxlite (tiled editor loader) as a dependency
FetchContent_Declare(
    TMXLITE
    GIT_REPOSITORY https://github.com/fallahn/tmxlite.git
    GIT_TAG v1.4.4
    SOURCE_SUBDIR tmxlite)
FetchContent_MakeAvailable(TMXLITE)


set(PROJECT_HEADERS 

    include/Core/Game.h
    include/Core/World.h
    include/Core/Level.h

    include/Gameplay/EconomyManager.h
    include/Gameplay/Enemy.h
    include/Gameplay/EnemyManager.h
    include/Gameplay/GameObject.h
    include/Gameplay/Goblin.h
    include/Gameplay/Tower.h
    include/Gameplay/Projectile.h

    include/Render/SFMLOrthogonalLayer.h

    include/UI/MainMenu.h    
    include/UI/Interface.h    

    include/Utils/AssetManager.h
    include/Utils/Constants.h 
    include/Utils/Tools.h
)

set(PROJECT_SOURCES 

    src/Core/Game.cpp
    src/Core/World.cpp
    src/Core/Level.cpp
    
    src/Gameplay/EconomyManager.cpp
    src/Gameplay/Enemy.cpp
    src/Gameplay/EnemyManager.cpp
    src/Gameplay/Goblin.cpp
    src/Gameplay/Tower.cpp
    src/Gameplay/Projectile.cpp

    src/UI/MainMenu.cpp   
    src/UI/Interface.cpp   

    src/Utils/AssetManager.cpp 
    src/Utils/Tools.cpp

    src/main.cpp
)

set(PROJECT_FILES 
  ${PROJECT_HEADERS} 
  ${PROJECT_SOURCES}
)

include_directories(${PROJECT_NAME} PRIVATE ${tmxlite_INCLUDE_DIR} include)

add_executable(${PROJECT_NAME} ${PROJECT_FILES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics tmxlite)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_compile_definitions(${PROJECT_NAME} PUBLIC DEBUG_MODE=0)

if(WIN32)
    add_custom_command(
        TARGET ${PROJECT_NAME}
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:${PROJECT_NAME}>
        VERBATIM)
endif()
