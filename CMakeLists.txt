cmake_minimum_required(VERSION 3.18)   

project(HOG_descriptor VERSION 1.0 LANGUAGES CXX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "cmake")

include(GNUInstallDirs)
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-anon-enum-enum-conversion")
endif()

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(CCN_DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(CCN_DOXYGEN_OUT ${CMAKE_BINARY_DIR}/Doxyfile)
    configure_file(${CCN_DOXYGEN_IN} ${CCN_DOXYGEN_OUT})
    add_custom_target(hoglib_doc_doxygen ALL
            COMMAND ${DOXYGEN_EXECUTABLE} ${CCN_DOXYGEN_OUT}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            VERBATIM)
    install (DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/hoglib_doc
        DESTINATION ./bin)
else ()
    message(STATUS "Couldn't find Doxygen")
endif ()

find_package(OpenCV CONFIG REQUIRED)
find_package(doctest REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )
message("OpenCV_INCLUDE_DIRS: " ${OpenCV_INCLUDE_DIR})

if(WIN32)
    set(OpenCV_RUNTIME_PATH "${OpenCV_DIR}/../../bin")
else()
    set(OpenCV_RUNTIME_PATH "${OpenCV_DIR}/../../lib")
endif()

install(DIRECTORY "${OpenCV_RUNTIME_PATH}/"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}
        FILES_MATCHING PATTERN "*opencv_core*"
        )

install(FILES hogconfig.txt DESTINATION ./bin)

add_subdirectory(src)