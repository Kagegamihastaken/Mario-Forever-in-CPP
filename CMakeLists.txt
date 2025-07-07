cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0048 NEW)
cmake_policy(SET CMP0069 NEW)
project(MFCPP VERSION 0.0.1.0)

# --- Global Project Settings ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
#TODO FORCE DEVELOPMENT_BUILD ON WHEN BUILD WITH RelWithDebInfo
set(DEVELOPMENT_BUILD ON)
set(LOGGING_ENABLE ON)

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
        GIT_PROGRESS ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_Declare(CTRE
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG v3.9.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(physfs
        GIT_REPOSITORY https://github.com/icculus/physfs.git
        GIT_TAG release-3.2.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(cpptrace
        GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
        GIT_TAG v1.0.2
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 11.2.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.91.9b
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(imgui-sfml
        GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
        GIT_TAG v3.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(Boost
        GIT_REPOSITORY https://github.com/boostorg/boost.git
        GIT_TAG boost-1.88.0
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
FetchContent_Declare(soloud
        GIT_REPOSITORY https://github.com/jarikomppa/soloud.git
        GIT_TAG RELEASE_20200207
        GIT_SHALLOW ON
        GIT_PROGRESS ON
        SYSTEM
)
#sfml
message("Making dependencies available...")
FetchContent_MakeAvailable(SFML CTRE physfs fmt cpptrace)
#soloud
FetchContent_MakeAvailable(soloud)
file(GLOB_RECURSE SOLOUD_SOURCES CONFIGURE_DEPENDS
        "${soloud_SOURCE_DIR}/src/audiosource/*.cpp"
        "${soloud_SOURCE_DIR}/src/audiosource/*.c"
        "${soloud_SOURCE_DIR}/src/c_api/*.cpp"
        "${soloud_SOURCE_DIR}/src/c_api/*.c"
        "${soloud_SOURCE_DIR}/src/core/*.cpp"
        "${soloud_SOURCE_DIR}/src/core/*.c"
        "${soloud_SOURCE_DIR}/src/filter/*.cpp"
        "${soloud_SOURCE_DIR}/src/filter/*.c"
)
if(WIN32)
    file(GLOB_RECURSE SOLOUD_BACKEND
            "${soloud_SOURCE_DIR}/src/backend/miniaudio/*.cpp"
    )
    set(SOLOUD_DEFINITIONS WITH_MINIAUDIO)
else()
    # You would need to add logic for other platforms here
    # For example, for Linux with ALSA:
    # file(GLOB SOLOUD_BACKEND_SOURCES "${soloud_SOURCE_DIR}/src/backend/alsa/*.cpp")
    # set(SOLOUD_DEFINITIONS WITH_ALSA)
    message(FATAL_ERROR "SoLoud build: No backend selected for this platform.")
endif()
add_library(soloud_library STATIC
        ${SOLOUD_SOURCES}
        ${SOLOUD_BACKEND}
)
target_include_directories(soloud_library PUBLIC
        "${soloud_SOURCE_DIR}/include"
        "${soloud_SOURCE_DIR}/src"
)
target_compile_definitions(soloud_library PRIVATE ${SOLOUD_DEFINITIONS})
add_library(SoLoud::soloud ALIAS soloud_library)
#libmodplug

FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
option(IMGUI_SFML_FIND_SFML "Use find_package to find SFML" OFF)
option(IMGUI_SFML_IMGUI_DEMO "Build imgui_demo.cpp" OFF)
FetchContent_MakeAvailable(imgui-sfml)

set(BOOST_INCLUDE_LIBRARIES container_hash uuid type_index)
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
    <unordered_map> <fmt/core.h> <fmt/base.h> <fmt/color.h> <exception>
    <fmt/format.h>
    #Boost category
    <boost/container_hash/hash.hpp> <boost/uuid.hpp> <boost/type_index.hpp>
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

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE -ffunction-sections -fdata-sections)
    target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,--gc-sections)
    # Apply aggressive optimization for non-development (production) release builds
    if(NOT DEVELOPMENT_BUILD)
        message(STATUS "Production Build detected. Applying -O3.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
    else()
        message(STATUS "Development Build detected. Applying -O2.")
        target_compile_options(${CMAKE_PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)
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
)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    SFML::Main
    SFML::Graphics SFML::System SFML::Window
    physfs-static ctre::ctre ImGui-SFML::ImGui-SFML
    Boost::container_hash Boost::uuid Boost::type_index
    fmt::fmt cpptrace::cpptrace SoLoud::soloud
)

# Link platform-specific system libraries
if(WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE winmm gdi32 user32 shell32 ole32 uuid)
    if(MINGW)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -static -static-libgcc -static-libstdc++)
        if (NOT DEVELOPMENT_BUILD)
            target_link_options(${CMAKE_PROJECT_NAME} PRIVATE -Wl,-subsystem,windows)
        endif()
    endif()
endif()

# --- Post-Build and Debugger Settings ---
# Copy the libopenmpt DLL to the executable's directory after a successful build
#add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_if_different
#        "${CMAKE_CURRENT_SOURCE_DIR}/data.zip"
#        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
#        COMMENT "Copying data.zip to build directory..."
#)
#add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        "${CMAKE_CURRENT_SOURCE_DIR}/musics"
#        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/musics"
#        COMMENT "Copying music files to build directory..."
#)

# Set the working directory for debugging in Visual Studio
set_property(TARGET ${CMAKE_PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
