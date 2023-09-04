#pragma once

#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

struct Texture2D {
  uint32_t width{ 0 };
  uint32_t height{ 0 };
  VkFormat imageFormat{};

  std::vector<uint8_t> pixels{};

  Texture2D() = default;
  Texture2D( uint32_t width, uint32_t height, std::vector<uint8_t> pixels, VkFormat format = VK_FORMAT_R8G8B8A8_SRGB );

  static auto FromFile( const std::string& filename ) -> Texture2D;
};

}  // namespace EA::Rendering