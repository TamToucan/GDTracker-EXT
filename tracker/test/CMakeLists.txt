SET(TESTEXE trackerTest)
add_executable(${TESTEXE} main.cpp)

# Include 'src' so it can see core headers
target_include_directories(${TESTEXE} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/../src")

# Link against the Core library
target_link_libraries(${TESTEXE} PRIVATE TrackerLib::Tracker)

# Ensure DLLs are present for Windows execution
if(TARGET TrackerLib::Tracker)
    add_custom_command(TARGET ${TESTEXE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:TrackerLib::Tracker>
        $<TARGET_FILE_DIR:${TESTEXE}>
    )
endif()
