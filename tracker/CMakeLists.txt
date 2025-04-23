project(GDTracker)

# Automatically pick up files added to src
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.C"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
        
IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
    SET(LIB_NAME GDTracker.release) 
ELSE()
    SET(LIB_NAME GDTracker.debug) 
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

add_library(${LIB_NAME} SHARED ${SOURCES})

target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
					                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_link_libraries(${LIB_NAME} PUBLIC godot::cpp)

add_custom_command(
        TARGET ${LIB_NAME}  POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${LIB_NAME}>
			    "D:/git/the-lott/addons/gdtracker/bin")

