#pragma once

#include <eavulkan/shared/includes.hpp>

template <class T>
constexpr void vkCheck( T func, const char* err ) {
  VkResult res = func;
  if ( res != VK_SUCCESS ) {
    throw std::runtime_error( fmt::format( "{}: {}", err, static_cast<uint32_t>( res ) ) );
  }
}

template <class T>
constexpr auto toU32( T res ) -> uint32_t {
  return static_cast<uint32_t>( res );
}

// Todo(emreaydn) refactor to read from any file and add one to read shader files
template <typename T>
auto readShaderFile( const std::string& shader_path ) -> std::vector<T> {
  std::ifstream ifs{ shader_path, std::ios::binary | std::ios::ate };
  if ( !ifs ) {
    throw std::runtime_error( fmt::format( "Failed to open shader file from {}", shader_path ) );
  }
  const auto size = ifs.tellg();
  ifs.seekg( 0, std::ios::beg );

  std::vector<T> buffer( size );

  if ( !ifs.read( reinterpret_cast<char*>( buffer.data() ), toU32( buffer.size() ) ) ) {
    throw std::runtime_error( fmt::format( "Failed to read shader file from {}", shader_path ) );
  }

  return buffer;
}
