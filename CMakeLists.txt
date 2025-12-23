cmake_minimum_required(VERSION 3.20)

# --- 1. STANDALONE SETUP (Only runs if not a submodule) ---
if(NOT ALL_SUBS_BUILD_ON)
    project(Tracker-LIB)
    
    # Standardizing on C++20 to match your other libraries
    set(CMAKE_CXX_STANDARD 20)
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    
    # Options
    if(DEFINED BUILD_GODOT)
        set(BUILD_GODOT_TRACKER ${BUILD_GODOT})
    else()
        option(BUILD_GODOT "Build Godot support" OFF)
        set(BUILD_GODOT_TRACKER ${BUILD_GODOT})
    endif()

    option(BUILD_CUTE "Build Cute Framework support" ON)

    include(FetchContent)
    set(FETCHCONTENT_BASE_DIR ${CMAKE_BINARY_DIR}/_deps)

    # 1. Fetch Common Libs
    FetchContent_Declare(
        Libs
        GIT_REPOSITORY https://github.com/TamToucan/Libs.git
        GIT_TAG main
    )

    # 2. Fetch Godot (Only if needed)
    if(BUILD_GODOT_TRACKER)
        FetchContent_Declare(
            GDExtension
            GIT_REPOSITORY https://github.com/godotengine/godot-cpp.git
            GIT_TAG godot-4.3-stable
        )
        FetchContent_MakeAvailable(GDExtension Libs)
    else()
        FetchContent_MakeAvailable(Libs)
    endif()

    # 3. Setup Cute (Standalone Mode)
    if(BUILD_CUTE)
        set(CUTE_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/deps/cute")
        if(EXISTS "${CUTE_ROOT}/lib/libcute.a")
            set(CUTE_FOUND ON)
            add_library(cute STATIC IMPORTED GLOBAL)
            set_target_properties(cute PROPERTIES
                IMPORTED_LOCATION "${CUTE_ROOT}/lib/libcute.a"
                INTERFACE_INCLUDE_DIRECTORIES "${CUTE_ROOT}/include;${CUTE_ROOT}"
            )
            file(GLOB CUTE_DEPENDENCIES "${CUTE_ROOT}/lib/*.a")
            list(REMOVE_ITEM CUTE_DEPENDENCIES "${CUTE_ROOT}/lib/libcute.a")
            set_target_properties(cute PROPERTIES INTERFACE_LINK_LIBRARIES "${CUTE_DEPENDENCIES}")
        endif()
    endif()

else()
    message(STATUS "Tracker-LIB is being built as a submodule.")
endif()


# --- 2. Build Core Library (The Fix) ---
# We point to the specific folder where we moved the core CMake logic
add_subdirectory(tracker/src/core)
add_library(TrackerLib::Tracker ALIAS Tracker)


# --- 3. Build Godot Adapter (GDTracker) ---
if(BUILD_GODOT OR BUILD_GODOT_TRACKER)
    add_subdirectory(tracker/src/godot)
endif()


# --- 4. Build Cute Adapter (CuteTracker) ---
if(BUILD_CUTE AND CUTE_FOUND)
    add_subdirectory(tracker/src/cute)
endif()


# --- 5. Build Tests ---
option(BUILD_TRACKER_TESTS "Build Tracker library tests" ON)

if(BUILD_TRACKER_TESTS)
    add_subdirectory(tracker/test)
endif()
