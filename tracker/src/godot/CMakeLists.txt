set(GDTRACKER_LIB_NAME GDTracker)

file(GLOB_RECURSE GDTRACKER_SOURCES CONFIGURE_DEPENDS
    "*.h" "*.hpp" "*.cpp"
)

add_library(${GDTRACKER_LIB_NAME} SHARED ${GDTRACKER_SOURCES})

target_include_directories(${GDTRACKER_LIB_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../core"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}/_deps/libs-src/lib/Util"
)

if(NOT MSVC)
    set_target_properties(${GDTRACKER_LIB_NAME} PROPERTIES PREFIX "")
endif()

# Link to the sibling 'tracker' target
target_link_libraries(${GDTRACKER_LIB_NAME}
    PRIVATE Tracker
    PUBLIC godot::cpp
)

# Copy DLLs to Godot Project
# Adjust "addons/gdtracker/bin" if your folder structure differs
add_custom_command(
    TARGET ${GDTRACKER_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:Tracker>
        "D:/git/the-lott/addons/gdtracker/bin"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:${GDTRACKER_LIB_NAME}>
        "D:/git/the-lott/addons/gdtracker/bin"
)
