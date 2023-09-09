#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <eavulkan/rendering/texture.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

Texture2D::Texture2D( uint32_t width, uint32_t height, std::vector<uint8_t> pixels, VkFormat format )
  : width( width ), height( height ), imageFormat( format ), pixels( std::move( pixels ) ) {}

auto Texture2D::FromFile( const std::string& filename ) -> Texture2D {
  int texWidth{};
  int texHeight{};
  int texChannels{};
  auto* pixels = stbi_load( filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha );
  if ( pixels == nullptr ) {
    throw std::runtime_error( "Failed to load texture image!" );
  }

  auto pixelsLength = texWidth * texHeight * 4;
  const std::span pixelsSpan( pixels, pixelsLength );
  const std::vector<uint8_t> pixelsVec( pixelsSpan.begin(), pixelsSpan.end() );
  stbi_image_free( pixels );

  return { toU32( texWidth ), toU32( texHeight ), pixelsVec };
}

}  // namespace EA::Rendering
