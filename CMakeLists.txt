cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)
project(MFCPP VERSION 0.0.1.0)

# --- Global Project Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

set(DEVELOPMENT_BUILD OFF)

#Game Config
set(PLAYER_LIVES 4)
set(GAME_WIDTH 640)
set(GAME_HEIGHT 480)

#lib include
include(FetchContent)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SFML_STATIC_LIBRARIES TRUE)
#dependency
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_Declare(CTRE
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG v3.9.0
        GIT_SHALLOW ON
        SYSTEM
)
FetchContent_Declare(physfs
        GIT_REPOSITORY https://github.com/icculus/physfs.git
        GIT_TAG release-3.2.0  # You can specify a specific version/tag
        GIT_SHALLOW ON
        SYSTEM
)
FetchContent_Declare(libmodplug
        GIT_REPOSITORY https://github.com/Konstanty/libmodplug.git
        GIT_TAG master # Pinned from master
        GIT_SHALLOW ON
        SYSTEM
)
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b
        GIT_SHALLOW ON
        SYSTEM
)
FetchContent_Declare(imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v3.0
        GIT_SHALLOW ON
        SYSTEM
)
FetchContent_Declare(Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.88.0
        GIT_SHALLOW ON
        SYSTEM
)
#sfml
message("Making dependencies available...")
FetchContent_MakeAvailable(SFML CTRE physfs)
#libmodplug
set(MODPLUG_STATIC ON)
set(MODPLUG_BUILD_SHARED OFF)
if (MSVC)
    set(HAVE_X87 ON)
endif()
FetchContent_MakeAvailable(libmodplug)

FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" OFF)
FetchContent_MakeAvailable(imgui-sfml)

set(BOOST_INCLUDE_LIBRARIES container_hash)
set(BOOST_ENABLE_CMAKE ON)
FetchContent_MakeAvailable(Boost)

# --- Project Sources and Executable ---
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/config/config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/config/config.h
)
#source project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source.cpp"
)
#header project
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
)

add_executable(${CMAKE_PROJECT_NAME} ${MY_SOURCES} ${MY_HEADERS})
# Use precompiled headers to speed up compilation of common, stable headers.
target_precompile_headers(${CMAKE_PROJECT_NAME} PRIVATE
    <SFML/Audio.hpp> <SFML/Graphics.hpp> <vector> <map> <optional>
    <string> <set> <array> <iostream> <algorithm>
    <random> <utility> <ctre.hpp> <physfs.h> <fstream>
    #Boost category
    <boost/container_hash/hash.hpp>
)
# --- Target-Specific Properties ---
if(WIN32)
    # Ensure all four version components are defined for the resource compiler.
    if(NOT DEFINED PROJECT_VERSION_TWEAK)
        set(PROJECT_VERSION_TWEAK 0)
    endif()
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/Resource.rc.in"
            "${CMAKE_CURRENT_BINARY_DIR}/Resource.rc"
            @ONLY
    )
    if(MINGW)
        enable_language(RC)
    endif()
    target_sources(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/Resource.rc")
endif()
# Set compiler definitions
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE MODPLUG_STATIC HAVE_X87)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--gc-sections)
    # Apply aggressive optimization for non-development (production) release builds
    if(NOT DEVELOPMENT_BUILD)
        message(STATUS "Production Build detected. Applying -O3.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    else()
        message(STATUS "Development Build detected. Applying -O2.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O2>)
    endif()
endif()

if(MSVC)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
            _CRT_SECURE_NO_WARNINGS _UNICODE UNICODE _CRT_NON_CONFORMING_WCSTOK
    )
    if (NOT DEVELOPMENT_BUILD)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /SUBSYSTEM:WINDOWS)
    else()
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE /SUBSYSTEM:CONSOLE)
    endif()
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"
        "${CMAKE_CURRENT_BINARY_DIR}/config"
        "${libmodplug_SOURCE_DIR}/src"
        "${libmodplug_SOURCE_DIR}/src/libmodplug" # for headers in sources directory
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SFML::Main
    SFML::Audio SFML::Graphics SFML::System SFML::Window
    physfs-static modplug ctre::ctre ImGui-SFML::ImGui-SFML Boost::container_hash
)

# Link platform-specific system libraries
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE gdi32 user32 shell32 ole32 uuid)
    if(MINGW)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
        if (NOT DEVELOPMENT_BUILD)
            target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,-subsystem,windows)
        endif()
    endif()
endif()

# --- Post-Build and Debugger Settings ---
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/data.zip"
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
        COMMENT "Copying data.zip to build directory..."
)
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/musics"
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/musics"
        COMMENT "Copying music files to build directory..."
)

# Set the working directory for debugging in Visual Studio
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
