add_executable(hogexe main.cpp)

# Link libraries
target_link_libraries(hogexe PRIVATE hoglibrary)
target_link_libraries(hogexe PRIVATE ${OpenCV_LIBS})

# Set the include directories for hoglib headers
target_include_directories(hogexe PRIVATE
    ${CMAKE_SOURCE_DIR}/prj.cw/src/lib/hogdescriptor/include
    ${CMAKE_SOURCE_DIR}/prj.cw/src/lib/texvisualization/include
)

#Define a constant variable for main.cpp code
target_compile_definitions(hogexe PRIVATE INSTALL_PATH="${CMAKE_INSTALL_PREFIX}/bin")

#Install hogexe executable into a ./bin folder
install(TARGETS hogexe RUNTIME DESTINATION ./bin
        COMPONENT Runtime)

#Install all dll dependencies along with executable
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/
        DESTINATION ./bin
        COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll"
)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/
        DESTINATION ./bin
        COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll"
)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/
        DESTINATION ./bin
        COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll"
)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO}/
        DESTINATION ./bin
        COMPONENT Runtime
        FILES_MATCHING PATTERN "*.dll"
)

#Install configuration file and images for main.cpp console application
install(FILES hogconfig.txt DESTINATION ./bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/images DESTINATION ./bin)
