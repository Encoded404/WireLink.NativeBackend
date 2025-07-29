cmake_minimum_required(VERSION 3.21..4.0)

project(WireLinkBackend
    VERSION 0.1.0
    DESCRIPTION "WireLink Backend"
    LANGUAGES CXX)

option(TREAT_WARNINGS_AS_ERROR "Treat warnings as error." ON)

option(BUILD_TESTING "Build the testing tree." ON)
option(WIRELINKBACKEND_FORCE_BUILD_TESTING "Force build testing even if not the top-level project." OFF)

option(BUILD_EXAMPLES "Build examples" OFF)

# Only configure these settings if this is the main project
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)

    # Enable ccache if available for faster builds
    find_program(CCACHE_PROGRAM ccache)
    if(CCACHE_PROGRAM)
        set(CMAKE_CXX_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    endif()

    include(CTest)

    # Enable compiler warnings for development
    set(CMAKE_COMPILE_WARNING_AS_ERROR ON)
endif()

find_package(Threads REQUIRED)

# The library code
add_subdirectory(src)

# Include examples/apps only if this is the main project
if(${PROJECT_IS_TOP_LEVEL} AND BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Include testing only if this is the main project and testing is enabled OR if the overwrite option is set
if(WIRELINKBACKEND_FORCE_BUILD_TESTING OR ${PROJECT_IS_TOP_LEVEL} AND BUILD_TESTING)
    add_subdirectory(tests)
endif()