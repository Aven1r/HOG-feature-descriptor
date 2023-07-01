# Determine the target platform (For library extension identification)
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(TargetPlatform "Windows")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  set(TargetPlatform "macOS")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(TargetPlatform "Linux")
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

# Set the default cross-compiler based on the target platform
if(TargetPlatform STREQUAL "Windows")
  set(CMAKE_C_COMPILER "x86_64-w64-mingw32-gcc")
  set(CMAKE_CXX_COMPILER "x86_64-w64-mingw32-g++")
elseif(TargetPlatform STREQUAL "macOS")
  set(CMAKE_C_COMPILER "/usr/bin/clang")
  set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
elseif(TargetPlatform STREQUAL "Linux")
  set(CMAKE_C_COMPILER "/usr/bin/gcc")
  set(CMAKE_CXX_COMPILER "/usr/bin/g++")
endif()

# Set the Vcpkg triplet based on the target platform
if(TargetPlatform STREQUAL "Windows")
  set(VcpkgTargetTriplet "x64-windows")
elseif(TargetPlatform STREQUAL "macOS")
  set(VcpkgTargetTriplet "x64-osx")
elseif(TargetPlatform STREQUAL "Linux")
  set(VcpkgTargetTriplet "x64-linux")
endif()

# Set the path to the Vcpkg toolchain file
set(VcpkgToolchainFile "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")

# Print the platform and compiler information
message(STATUS "Target Platform: ${TargetPlatform}")
message(STATUS "C Compiler: ${CMAKE_C_COMPILER}")
message(STATUS "C++ Compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "Vcpkg Target Triplet: ${VcpkgTargetTriplet}")
message(STATUS "Vcpkg Toolchain File: ${VcpkgToolchainFile}")

# Configure the project using the Vcpkg toolchain file
set(CMAKE_TOOLCHAIN_FILE ${VcpkgToolchainFile} CACHE STRING "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin.dbg)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin.rel)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${CMAKE_SOURCE_DIR}/bin.relwithdbg)