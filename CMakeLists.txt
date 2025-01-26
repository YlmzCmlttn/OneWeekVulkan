cmake_minimum_required(VERSION 3.10)

# Project Name
project(VulkanProject)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Define Vulkan SDK Path (Adjust based on your system)
find_package(Vulkan REQUIRED)

# Add GLFW and GLM directories
set(THIRD_PARTY_DIR ${CMAKE_SOURCE_DIR}/3rdParty)
set(GLFW_DIR ${THIRD_PARTY_DIR}/GLFW)
set(GLM_DIR ${THIRD_PARTY_DIR}/GLM)

# Include GLFW and GLM headers
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})

# Add Source Files
file(GLOB_RECURSE SOURCES src/*.cpp)

# Add Executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link GLFW and Vulkan Libraries
find_library(GLFW_LIB glfw3 PATHS ${GLFW_DIR}/lib NO_DEFAULT_PATH)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan ${GLFW_LIB})

# Platform-Specific Dependencies
if(WIN32)
    target_link_libraries(${PROJECT_NAME} opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    target_link_libraries(${PROJECT_NAME} ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREVIDEO_LIBRARY})
elseif(UNIX)
    find_package(X11 REQUIRED)
    target_link_libraries(${PROJECT_NAME} X11 pthread dl)
endif()

# Output Directory for Binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
