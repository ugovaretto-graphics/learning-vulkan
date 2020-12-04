# Author: Ugo Varetto
cmake_minimum_required(VERSION 3.1...3.17)

if(${CMAKE_VERSION} VERSION_LESS 3.16)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.16)
endif()
project(vulkan-tutorial VERSION 1.0
                   DESCRIPTION "My Vulkan Tutorial"
                   LANGUAGES CXX)

# Separate debug-release paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin/debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin/release)

#packages
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

find_package(Vulkan)

if(NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan not found")
endif()

set(INCLUDE_DIRS ${Vulkan_INCLUDE_DIRS})
set(LIBS ${Vulkan_LIBRARY})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_definitions(VK_USE_PLATFORM_XLIB_KHR)

include_directories(${INCLUDE_DIRS})
add_subdirectory(src)