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

target_compile_definitions(${LIB_NAME} PRIVATE ${PROJECT_NAME}_EXPORTS)

target_include_directories(${LIB_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src"
					                   PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/inc")
target_link_libraries(${LIB_NAME} PUBLIC godot::cpp)

##################################
# NOTE: We DONT copy this to the addon dir.
# The dll here is somehow difference from the one created and linked with
# the gddistanceMap.dll and so g_tracker is different
# The dll build here: GET GLOBAL: 0x7ffbf7735020
#   GDDistanceMap::setTracker is called
# The g_tracker it gets from GDTracker::getInstance: 0x7ffbfaf96020 => null for tracker
#

# Copy it to the gddistanceMap since it's dll looks for it in the same directory
#set(LOTT_ADON_DIR "D:/git/the-lott/addons/gddistanceMap/bin")
#add_custom_command(
        #TARGET ${LIB_NAME}  POST_BUILD
        #COMMAND ${CMAKE_COMMAND} -E make_directory
        #${LOTT_ADON_DIR}
        #COMMAND ${CMAKE_COMMAND} -E copy
                #${CMAKE_CURRENT_BINARY_DIR}/$<TARGET_FILE_NAME:${LIB_NAME}>
                #${LOTT_ADON_DIR})

