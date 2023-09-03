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
        instance.cpp
        physical_device.cpp
        queue.cpp
        device.cpp
        ${HEADER_LIST}
)
#        VRenderPass.cpp
#        VPipeline.cpp
#        VBuffer.cpp
#        VImage.cpp
#        VSwapchain.cpp
#        VResource.cpp
#        VCommandBuffer.cpp
#        VCommandPool.cpp
#        VRenderPass.cpp
#        VPipeline.cpp
#        VFramebuffer.cpp
#        VDescriptorPool.cpp
#        VDescriptorSetLayout.cpp
#        VDescriptorSet.cpp)
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