cmake_minimum_required(VERSION 3.25)
project(MFCPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Ship game
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#lib include
include(FetchContent)

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)
set(SFML_STATIC_LIBRARIES TRUE)
#sfml
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)
#ctre
FetchContent_Declare(CTRE
        GIT_REPOSITORY https://github.com/hanickadot/compile-time-regular-expressions.git
        GIT_TAG v3.9.0
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(CTRE)
#physfs
FetchContent_Declare(physfs
        GIT_REPOSITORY https://github.com/icculus/physfs.git
        GIT_TAG release-3.2.0  # You can specify a specific version/tag
        GIT_SHALLOW ON
)
FetchContent_MakeAvailable(physfs)

#libmodplug
set(MODPLUG_STATIC ON CACHE BOOL "Build static modplug library" FORCE)
set(MODPLUG_BUILD_SHARED OFF CACHE BOOL "Build shared modplug library" FORCE)

FetchContent_Declare(libmodplug
        GIT_REPOSITORY https://github.com/Konstanty/libmodplug.git
        GIT_TAG master  # You might want to use a specific commit hash for stability
        GIT_SHALLOW ON
)
#FetchContent_MakeAvailable(libmodplug)
FetchContent_GetProperties(libmodplug)
if(NOT libmodplug_POPULATED)
    if(MSVC)
        file(WRITE "${libmodplug_BINARY_DIR}/config.h" "#define HAVE_X87 1\n")
    endif()
    FetchContent_MakeAvailable(libmodplug)
    #(libmodplug, ${libmodplug_SOURCE_DIR})
    #add_subdirectory(libmodplug, ${libmodplug_BINARY_DIR})
endif()

#source project
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source.cpp"
)

message("Source files to be compiled:")
foreach(source ${MY_SOURCES})
    message("  ${source}")
endforeach()

#header project
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
)
message("Source files to be compiled:")
foreach(header ${MY_HEADERS})
    message("  ${header}")
endforeach()

#compile icon program and data
if(WIN32)
    set(APP_RESOURCE_WINDOWS
            "${CMAKE_CURRENT_SOURCE_DIR}/Resource.rc"
    )
    if(MSVC)
        # MSVC specific RC handling
        set_source_files_properties(${APP_RESOURCE_WINDOWS} PROPERTIES
                COMPILE_FLAGS "/nologo"
        )
    elseif(MINGW)
        # Ensure proper resource compilation with MinGW
        set(CMAKE_RC_COMPILER_INIT windres)
        enable_language(RC)
        set(CMAKE_RC_COMPILE_OBJECT
                "<CMAKE_RC_COMPILER> <FLAGS> -O coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    endif()
endif()

add_executable(${CMAKE_PROJECT_NAME}
        ${MY_SOURCES}
        ${MY_HEADERS}
        ${APP_RESOURCE_WINDOWS}

)
#data
set(RESOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/data.zip"
)

# music
set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/musics")
if(NOT PRODUCTION_BUILD)
    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCE_FILES}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)

    add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${MUSIC_DIR}
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/musics"
)
else()
    if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(RELEASE_DIR "${CMAKE_BINARY_DIR}/Release")
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${RELEASE_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RESOURCE_FILES}
            "${RELEASE_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${MUSIC_DIR}
            "${RELEASE_DIR}/musics"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
            "${RELEASE_DIR}"
        )
        target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
        target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0)
    else()
        set(DEBUG_DIR "${CMAKE_BINARY_DIR}/Debug")
        add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E make_directory "${DEBUG_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${RESOURCE_FILES}
            "${DEBUG_DIR}"
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${MUSIC_DIR}
            "${DEBUG_DIR}/musics"
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "$<TARGET_FILE:${CMAKE_PROJECT_NAME}>"
            "${DEBUG_DIR}"
        )

        target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
        target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1)
    endif()
endif()

# Then modify your target_sources to include both .rc files

if(MSVC)
    target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
            _CRT_SECURE_NO_WARNINGS
            _UNICODE
            UNICODE
            _CRT_NON_CONFORMING_WCSTOK
    )

    # Optional: Add resource compiler definitions if needed
    set_property(SOURCE ${APP_RESOURCE_WINDOWS} PROPERTY COMPILE_DEFINITIONS
            "NDEBUG;_UNICODE;UNICODE"
    )
endif()

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        MODPLUG_STATIC
        HAVE_X87
)

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"
        "${libmodplug_SOURCE_DIR}/src"
        "${libmodplug_SOURCE_DIR}/src/libmodplug" # for headers in sources directory
        "${libmodplug_BINARY_DIR}"
)
if(WIN32)
    if(MINGW)
        target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
                -static-libgcc
                -static-libstdc++
        )
    endif()
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE
            SFML::Audio SFML::Graphics SFML::System SFML::Window
            gdi32 user32 shell32 ole32 uuid
            physfs-static
            modplug ctre::ctre)
else()
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Audio SFML::Graphics SFML::System SFML::Window
            physfs-static
            modplug ctre::ctre)
endif()

set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES
        VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
)
