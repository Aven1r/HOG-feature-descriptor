add_library(hogdescriptor hogdescriptor.cpp include/hogdescriptor/hogdescriptor.hpp)
set_property(TARGET hogdescriptor PROPERTY CXX_STANDARD 20)
target_include_directories(hogdescriptor PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
install(TARGETS hogdescriptor)