SET(CUTETRACKER_LIB_NAME CuteTracker)

file(GLOB_RECURSE CUTETRACKER_SOURCES CONFIGURE_DEPENDS
    "*.h" "*.hpp" "*.cpp"
)

add_library(${CUTETRACKER_LIB_NAME} SHARED ${CUTETRACKER_SOURCES})

target_include_directories(${CUTETRACKER_LIB_NAME} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/../core"
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_BINARY_DIR}/_deps/libs-src/lib/Util"
)

if(NOT MSVC)
    set_target_properties(${CUTETRACKER_LIB_NAME} PROPERTIES PREFIX "")
endif()

# Link to the sibling 'Tracker' target
target_link_libraries(${CUTETRACKER_LIB_NAME}
	PRIVATE Tracker cute_lib)

# Windows: Copy dependencies next to CuteTracker so tests can run
add_custom_command(TARGET ${CUTETRACKER_LIB_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:Tracker>
    $<TARGET_FILE_DIR:${CUTETRACKER_LIB_NAME}>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:cute>
    $<TARGET_FILE_DIR:${CUTETRACKER_LIB_NAME}>
)



