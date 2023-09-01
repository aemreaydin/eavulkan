#ifndef common_h
#define common_h

#include <SDL.h>
#include <SDL_vulkan.h>
#include <gsl/gsl>
#include <spdlog/spdlog.h>
#include <tiny_obj_loader.h>
#include <vulkan/vulkan.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <set>
#include <span>
#include <vector>

template <class T> constexpr void VK_CHECK( T func, const char* err )
{
    VkResult res = func;
    if ( res != VK_SUCCESS )
    {
        throw std::runtime_error( fmt::format( "{}: {}", err, static_cast<uint32_t>( res ) ) );
    }
}

template <class T> constexpr uint32_t TO_U32( T res )
{
    return static_cast<uint32_t>( res );
}

// Todo(emreaydn) refactor to read from any file and add one to read shader files
template <typename T> std::vector<T> readFromFile( const std::string& shaderPath )
{
    std::ifstream ifs{ shaderPath, std::ios::binary | std::ios::ate };
    if ( !ifs )
    {
        throw std::runtime_error( fmt::format( "Failed to open shader file from {}", shaderPath ) );
    }
    const auto size = ifs.tellg();
    ifs.seekg( 0, std::ios::beg );

    std::vector<T> buffer( size );

    if ( !ifs.read( reinterpret_cast<char*>( buffer.data() ),
                    static_cast<uint32_t>( buffer.size() ) ) )
    {
        throw std::runtime_error( fmt::format( "Failed to read shader file from {}", shaderPath ) );
    }

    return buffer;
}

#endif