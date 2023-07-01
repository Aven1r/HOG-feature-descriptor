cmake_minimum_required(VERSION 3.18)   

project(hoglib)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(build_prefix)

# Add build flags for documentation and sample program
set(SAMPLE $ENV{SAMPLE})
set(DOXYGEN $ENV{DOXYGEN})

# Check if SAMPLE variable is set
if(NOT SAMPLE)
    set(SAMPLE OFF)
endif()

# Check if DOXYGEN variable is set
if(NOT DOXYGEN)
    set(DOXYGEN OFF)
endif()

# Include Doxygen configuration if DOXYGEN is enabled
if(DOXYGEN)
    include(doxygen_config)
endif()

# Compiler-specific flags
include(GNUInstallDirs)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-anon-enum-enum-conversion")
endif()

# Enable additional warnings for GCC
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
endif(CMAKE_COMPILER_IS_GNUCXX)

find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(src)

if(SAMPLE)
    add_subdirectory(application)
endif()