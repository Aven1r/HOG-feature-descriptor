add_library(texvisualization texvisualization.cpp include/texvisualization/texvisualization.hpp)
set_property(TARGET texvisualization PROPERTY CXX_STANDARD 20)
target_include_directories(texvisualization PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS texvisualization)
