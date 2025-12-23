# Target Name
set(TRACKER_LIB_NAME Tracker)

# Source Files
file(GLOB_RECURSE TRACKER_SOURCES CONFIGURE_DEPENDS
    "*.h" "*.hpp" "*.cpp"
)

# Define Shared Library
add_library(${TRACKER_LIB_NAME} SHARED ${TRACKER_SOURCES})

# Include Directories
target_include_directories(${TRACKER_LIB_NAME} PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"    # . (src/core)
    "${CMAKE_CURRENT_SOURCE_DIR}/.." # .. (src)
    # Common Deps
    "${CMAKE_BINARY_DIR}/_deps/libs-src/lib/MathStuff"
    "${CMAKE_BINARY_DIR}/_deps/libs-src/lib/Algo"
    "${CMAKE_BINARY_DIR}/_deps/libs-src/lib/Util"
)

# Properties
set_target_properties(${TRACKER_LIB_NAME} PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
if(NOT MSVC)
    set_target_properties(${TRACKER_LIB_NAME} PROPERTIES PREFIX "")
endif()

# Linking (Tracker likely depends on Algo/Math)
target_link_libraries(${TRACKER_LIB_NAME}
    PRIVATE Algo MathStuff
)
