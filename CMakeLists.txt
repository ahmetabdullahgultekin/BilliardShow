cmake_minimum_required(VERSION 3.15) # 3.15+ is enough; no need for 3.31 unless you require new features
project(BilliardShow)

set(CMAKE_CXX_STANDARD 20) # 26 is not yet officially released; use 20 for safety and compatibility

# vcpkg toolchain
set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
        CACHE STRING "Vcpkg toolchain file")

file(GLOB_RECURSE SOURCES
        "${CMAKE_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_SOURCE_DIR}/src/*.h"
        "${CMAKE_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_SOURCE_DIR}/src/*.c"
        "${CMAKE_SOURCE_DIR}/src/*.inl"
)

# vcpkg will provide the config packages for these
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

# With vcpkg, you do NOT need to manually add include_directories for these libraries
# Modern CMake uses targets (interface include dirs are linked via target_link_libraries)

add_executable(BilliardShow ${SOURCES})

#[[add_custom_command(TARGET BilliardShow POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:BilliardShow>/assets)]]

target_link_libraries(BilliardShow
        OpenGL::GL
        GLEW::GLEW
        glfw
        glm::glm
)