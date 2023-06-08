cmake_minimum_required(VERSION 3.18)   

project(HOG_descriptor VERSION 1.0 LANGUAGES CXX)

include(GNUInstallDirs)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")


find_package(OpenCV CONFIG REQUIRED)
find_package(doctest REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
install(FILES hogconfig.txt DESTINATION ./bin)

add_subdirectory(src)