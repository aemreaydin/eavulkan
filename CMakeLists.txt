cmake_minimum_required(VERSION 3.23)
project(eavulkan VERSION 0.0.1 DESCRIPTION "A Vulkan Engine for the ages" LANGUAGES CXX)

if (${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
    set(CMAKE_CXX_EXTENSIONS OFF)
    set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif ()

find_package(Vulkan REQUIRED)
find_package(sdl2 CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(tinyobjloader CONFIG REQUIRED)
find_package(Stb REQUIRED)

add_subdirectory(src)