add_executable(libExample libraryExample.cpp)
# Link libraries
target_link_libraries(libExample PRIVATE hoglibrary)
target_link_libraries(libExample PRIVATE ${OpenCV_LIBS})

# Set the include directories for hoglib headers
target_include_directories(libExample PRIVATE
    ${CMAKE_SOURCE_DIR}/src/lib/hogdescriptor/include
    ${CMAKE_SOURCE_DIR}/src/lib/texvisualization/include
)

install (FILES libraryExample.cpp DESTINATION ./example)