project(Tracker-Lib)

# Core tracker lib
set(TRACKER_LIB_NAME tracker)
file(GLOB_RECURSE TRACKER_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.C"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/core/*.cpp")
        
add_library(${TRACKER_LIB_NAME} SHARED ${TRACKER_SOURCES})
add_library(TrackerLib::Tracker ALIAS ${TRACKER_LIB_NAME})


set_target_properties(${TRACKER_LIB_NAME} PROPERTIES OUTPUT_NAME ${TRACKER_LIB_NAME}
        DEFINE_SYMBOL "Tracker_EXPORTS")

if(NOT MSVC)
	set_target_properties(${TRACKER_LIB_NAME} PROPERTIES PREFIX "")
endif()

target_include_directories(${TRACKER_LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/core>
    $<INSTALL_INTERFACE:include>
)

if(BUILD_GODOT_TRACKER)
##################
# Godot tracker lib
set(GDTRACKER_LIB_NAME GDTracker)
file(GLOB_RECURSE GDTRACKER_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/godot/*.h"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/godot/*.C"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/godot/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/godot/*.cpp")
        
add_library(${GDTRACKER_LIB_NAME} SHARED ${GDTRACKER_SOURCES})
add_library(TrackerLib::GDTracker ALIAS ${GDTRACKER_LIB_NAME})

set_target_properties(${GDTRACKER_LIB_NAME} PROPERTIES OUTPUT_NAME ${GDTRACKER_LIB_NAME}
	DEFINE_SYMBOL "GDTracker_EXPORTS")

if(NOT MSVC)
	set_target_properties(${GDTRACKER_LIB_NAME} PROPERTIES PREFIX "")
endif()


target_include_directories(${GDTRACKER_LIB_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/godot>
    $<INSTALL_INTERFACE:include>
)


# This was in before submodules
# target_compile_definitions(${LIB_NAME} PRIVATE ${PROJECT_NAME}_EXPORTS)

target_link_libraries(${GDTRACKER_LIB_NAME}
	PRIVATE TrackerLib::Tracker
	PUBLIC godot::cpp)
endif()
##################################
# Installation and Export

include(GNUInstallDirs)

# Install targets
# Install targets
install(TARGETS ${TRACKER_LIB_NAME}
    EXPORT TrackerLibTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(BUILD_GODOT_TRACKER)
install(TARGETS ${GDTRACKER_LIB_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
endif()

# Install headers
install(DIRECTORY src/core/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tracker/core FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
if(BUILD_GODOT_TRACKER)
install(DIRECTORY src/godot/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/tracker/godot FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")
endif()

# Export targets
install(EXPORT TrackerLibTargets
    FILE TrackerLibTargets.cmake
    NAMESPACE TrackerLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TrackerLib
)

# Create Config file (simplified)
include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/TrackerLibConfigVersion.cmake"
    VERSION 1.0.0
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/TrackerLibConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/TrackerLib
)
