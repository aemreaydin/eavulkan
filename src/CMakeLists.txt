file(GLOB_RECURSE HEADER_LIST CONFIGURE_DEPENDS
        "${CMAKE_SOURCE_DIR}/include/eavulkan/*.hpp")

set(THIRD_PARTY_INCLUDES
        ${Stb_INCLUDE_DIR})
set(THIRD_PARTY_LIBS
        Vulkan::Vulkan
        $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
        $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
        spdlog::spdlog
        Microsoft.GSL::GSL
        glm::glm
        tinyobjloader::tinyobjloader)

add_library(${CMAKE_PROJECT_NAME}
        rendering/instance.cpp
        rendering/physical_device.cpp
        rendering/queue.cpp
        rendering/device.cpp
        rendering/render_pass.cpp
        rendering/pipeline.cpp
        rendering/graphics_pipeline.cpp
        rendering/swap_chain.cpp
        rendering/descriptor_pool.cpp
        rendering/descriptor_set.cpp
        rendering/descriptor_set_layout.cpp
        rendering/command_buffer.cpp
        rendering/command_pool.cpp
        rendering/frame_buffer.cpp
        rendering/buffer.cpp
        rendering/image.cpp
        rendering/vertex.cpp
        rendering/mesh.cpp
        rendering/texture.cpp
        shared/transform.cpp
        shared/mvp.cpp
        model.cpp
        ${HEADER_LIST}
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${THIRD_PARTY_LIBS})
target_include_directories(${CMAKE_PROJECT_NAME}
        PUBLIC ../include
        PRIVATE ${THIRD_PARTY_INCLUDES}
)
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)