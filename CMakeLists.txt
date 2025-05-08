cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 17)

if(WIN32)
    # Windows-specific configurations
    add_definitions(-DWIN32_LEAN_AND_MEAN)     # Exclude rarely-used stuff from Windows headers
    add_definitions(-DNOMINMAX)                # Prevents Windows.h from defining min/max macros

    # If you need Common Controls v6
    add_definitions(-D_WIN32_WINNT=0x0601)    # Target Windows 7 or later

    # If you need Unicode support (though I see you're explicitly removing it in your current setup)
    # add_definitions(-DUNICODE -D_UNICODE)
endif()


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
#libmodplug
find_library(MODPLUG_LIBRARY
        NAMES modplug libmodplug
        PATHS "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/lib"
        REQUIRED
)
find_path(MODPLUG_INCLUDE_DIR
        NAMES modplug.h libmodplug/modplug.h
        PATHS
            "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/src"
            "${_VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/src/libmodplug"
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


add_executable("${CMAKE_PROJECT_NAME}")

set_property(TARGET "${CMAKE_PROJECT_NAME}" PROPERTY CXX_STANDARD 17)

if(PRODUCTION_BUILD)
    # setup the ASSETS_PATH macro to be in the root folder of your exe
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="./data/")
    # remove the option to debug asserts.
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=1)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=0)
else()
    # This is useful to get an ASSETS_PATH in your IDE during development
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC RESOURCES_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/")
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC PRODUCTION_BUILD=0)
    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC DEVELOPLEMT_BUILD=1)
endif()

if(WIN32)
    set(APP_RESOURCE_WINDOWS
            "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc"
            "${CMAKE_CURRENT_SOURCE_DIR}/Texture.rc"
    )
endif()

# Then modify your target_sources to include both .rc files
target_sources("${CMAKE_PROJECT_NAME}" PRIVATE
        ${MY_SOURCES}
        $<$<BOOL:${WIN32}>:${APP_RESOURCE_WINDOWS}>
)

if(MSVC) # If using the VS compiler...

    target_compile_definitions("${CMAKE_PROJECT_NAME}" PUBLIC _CRT_SECURE_NO_WARNINGS)

    #set_target_properties("${CMAKE_PROJECT_NAME}" PROPERTIES LINK_FLAGS "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup") #no console)

endif()

#force remove unicode
if (WIN32)
    target_compile_options("${CMAKE_PROJECT_NAME}" PRIVATE /UUNICODE /U_UNICODE)
endif()

target_include_directories("${CMAKE_PROJECT_NAME}" PRIVATE
        ${MODPLUG_INCLUDE_DIR}
        "${CMAKE_CURRENT_SOURCE_DIR}/headers"  # for headers in sources directory
)
if(WIN32)
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Audio SFML::Graphics SFML::System SFML::Window winmm gdi32 user32 shell32 ${MODPLUG_LIBRARY})
else()
    target_link_libraries("${CMAKE_PROJECT_NAME}" PRIVATE SFML::Audio SFML::Graphics SFML::System SFML::Window ${MODPLUG_LIBRARY})
endif()
