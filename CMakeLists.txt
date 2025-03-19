cmake_minimum_required(VERSION 3.10)
project(graphics2425_opengl_project VERSION 0.1.0 LANGUAGES C CXX)
cmake_policy(SET CMP0072 NEW)

# Force compiler selection
if (WIN32)
    if (NOT CMAKE_C_COMPILER)
        set(CMAKE_C_COMPILER "C:/msys64/mingw64/bin/gcc.exe" CACHE STRING "" FORCE)
        set(CMAKE_CXX_COMPILER "C:/msys64/mingw64/bin/g++.exe" CACHE STRING "" FORCE)
    endif()
endif()

# Detect OS
if(WIN32)
    message(STATUS "Compiling on Windows")
    set(assimp_DIR "${CMAKE_SOURCE_DIR}/assimp-windows/lib/cmake/assimp-5.4")
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/assimp-windows/include")
    set(ASSIMP_LIB_DIR "${CMAKE_SOURCE_DIR}/assimp-windows/lib")
else()
    message(STATUS "Compiling on Linux/Mac")
    set(assimp_DIR "${CMAKE_SOURCE_DIR}/assimp-linux-build/lib/cmake/assimp")
    set(ASSIMP_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/assimp-linux-build/include")
    set(ASSIMP_LIB_DIR "${CMAKE_SOURCE_DIR}/assimp-linux-build/lib")    
endif()

set(CMAKE_PREFIX_PATH "${assimp_DIR};${CMAKE_SOURCE_DIR}/assimp-linux-build/install")


find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

set(GLFW_BUILD_X11_XINERAMA OFF CACHE BOOL "" FORCE)

# Add GLFW
set(GLFW_PATH "${CMAKE_SOURCE_DIR}/GLFW")
add_subdirectory(${GLFW_PATH})

# Create executable
add_executable(graphics2425_opengl_project 
    src/main.cpp 
    src/glad.c
    src/Sphere.cpp
    src/stb_image.cpp
)

# Copy shaders to the build directory
add_custom_target(copy_shaders ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/src/shaders ${CMAKE_BINARY_DIR}/shaders
)

# Ensure the assets directory exists before copying
add_custom_target(copy_assets ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
)

# Make sure the files are copied before building the main executable
add_dependencies(graphics2425_opengl_project copy_shaders copy_assets)

# Include directories
target_include_directories(graphics2425_opengl_project PRIVATE 
    include 
    ${GLFW_PATH}/include/glfw
    ${ASSIMP_INCLUDE_DIR}
)

# Προσθήκη include directories
target_include_directories(graphics2425_opengl_project PRIVATE 
    include 
    ${GLFW_PATH}/include/glfw  # Προσθέτουμε απευθείας το glfw directory
)

# Link libraries
target_link_directories(graphics2425_opengl_project PRIVATE ${ASSIMP_LIB_DIR})

target_link_libraries(graphics2425_opengl_project 
    glfw
    ${OpenGL_GL_LIBRARY}
    assimp
)

# Copy Assimp DLL to build folder automatically
if(WIN32)
    add_custom_command(TARGET graphics2425_opengl_project POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${ASSIMP_LIB_DIR}/../bin/libassimp-5.dll"
        "$<TARGET_FILE_DIR:graphics2425_opengl_project>/libassimp-5.dll"
    )
endif()

message(STATUS "Using Assimp from: ${assimp_DIR}")

set_target_properties(graphics2425_opengl_project PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
)
