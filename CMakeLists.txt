cmake_minimum_required(VERSION 3.18)   

project(HOG_descriptor LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

set(CMAKE_VERBOSE_MAKEFILE True)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True) 

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_CURRENT_SOURCE_DIR}/bin.relwithdbg)

if (WIN32)
    set(OpenCV_DIR "Your path" CACHE PATH "Initial cache" FORCE)
elseif(UNIX)
    set(OpenCV_DIR "/Users/avenir/vcpkg/installed/arm64-osx/share/opencv" CACHE PATH "Initial cache" FORCE)
endif()

find_package(OpenCV CONFIG REQUIRED)
find_package(doctest REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(src)