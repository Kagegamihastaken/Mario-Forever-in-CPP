cmake_minimum_required(VERSION 3.31)

#Ship game
set(PRODUCTION_BUILD OFF CACHE BOOL "Make this a production build" FORCE)

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(PRODUCTION_BUILD)
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization
else()
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION FALSE)
endif()

set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build static libraries" FORCE)

if (MSVC)
    add_compile_options(/arch:AVX2) #make sure SIMD optimizations take place
endif()


project(MFCPP)
set(SFML_STATIC_LIBRARIES TRUE)
#sfml
include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.1
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)
#physfs
find_library(PHYSFS_LIBRARY
        NAMES physfs
        PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
)
find_path(PHYSFS_INCLUDE_DIR
        NAMES physfs.h
        PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
        REQUIRED
)


#libmodplug
find_library(MODPLUG_LIBRARY
        NAMES modplug libmodplug
        PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
)
find_path(MODPLUG_INCLUDE_DIR
        NAMES modplug.h libmodplug/modplug.h
        PATHS
            "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include"
            "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/include/libmodplug"
        REQUIRED
)



#add_subdirectory(lib/libmodplug)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/sources/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/Source.cpp"
)
file(GLOB_RECURSE MY_HEADERS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/headers/*.hpp"
)

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
        ${APP_RESOURCE_WINDOWS}
)
set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

set(RESOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/data.zip"
)

# Define the music directory
set(MUSIC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/musics")

add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${RESOURCE_FILES}
        $<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>
)

# Copy music directory to output directory
add_custom_command(TARGET ${CMAKE_PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${MUSIC_DIR}
        "$<TARGET_FILE_DIR:${CMAKE_PROJECT_NAME}>/musics"
)

if(PRODUCTION_BUILD)
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

    # setup the ASSETS_PATH macro to be in the root folder of your exe
    # remove the option to debug asserts.
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


    # This is useful to get an ASSETS_PATH in your IDE during development
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1)
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



target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE
        ${MODPLUG_INCLUDE_DIR}
        ${PHYSFS_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"  # for headers in sources directory
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
            ${PHYSFS_LIBRARY}
            ${MODPLUG_LIBRARY})
else()
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Audio SFML::Graphics SFML::System SFML::Window ${PHYSFS_LIBRARY}
            ${MODPLUG_LIBRARY})
endif()
