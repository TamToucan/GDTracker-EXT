cmake_minimum_required(VERSION 3.20)

if (NOT ALL_SUBS_BUILD_ON)
    project(gdextension-tracker)
    
    set(FETCHCONTENT_QUIET OFF)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
    
    set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)
    
    include(FetchContent)
    
    FetchContent_Declare(
            GDExtension
            GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
            GIT_TAG godot-4.3-stable
    )
 
    FetchContent_MakeAvailable(GDExtension)
else()
    message(STATUS "GDTracker-EXT is being built as part of Lott-EXT, skipping FetchContent.")
endif()

add_subdirectory(tracker)
