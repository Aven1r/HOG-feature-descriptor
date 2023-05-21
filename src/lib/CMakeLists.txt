project(HOG_descriptor_lib LANGUAGES CXX)

# List of all files in directory
set(SOURCES
        
        )

add_library(HOG_descriptor_lib ${SOURCES})

set_property(TARGET HOG_descriptor_lib PROPERTY CXX_STANDARD 20)