#pragma once

#include <eavulkan/rendering/resource.hpp>
#include <eavulkan/shared/includes.hpp>

namespace EA::Rendering {

struct SwapchainSurfaceSupportQuery {
  VkSurfaceCapabilitiesKHR surfaceCapabilities{};
  std::vector<VkPresentModeKHR> presentModes{};
  std::vector<VkSurfaceFormatKHR> surfaceFormats{};
};

class Swapchain : public DeviceResource<VkSwapchainKHR> {
  VkSurfaceKHR _surface{ nullptr };
  std::vector<VkImageView> _imageViews{};
  std::vector<VkImage> _images{};

  SwapchainSurfaceSupportQuery _surfaceSupportQuery{};
  VkSurfaceFormatKHR _surfaceFormat{};
  VkPresentModeKHR _presentMode{};
  VkExtent2D _extent{};
  uint32_t _imageCount{};

  auto createImageViews() -> void;

  [[nodiscard]] auto querySurfaceSupport() const -> SwapchainSurfaceSupportQuery;
  [[nodiscard]] auto useSuitableFormat() const -> VkSurfaceFormatKHR;
  [[nodiscard]] auto useSuitablePresentMode() const -> VkPresentModeKHR;
  [[nodiscard]] auto useSuitableExtent( VkExtent2D extent ) const -> VkExtent2D;

 public:
  explicit Swapchain( Device const& device, const VkSurfaceKHR& surface, VkExtent2D extent );

  [[nodiscard]] auto GetImageViews() const -> const std::vector<VkImageView>&;
  [[nodiscard]] auto GetImageView( uint32_t index ) const -> const VkImageView&;
  [[nodiscard]] auto GetImage( uint32_t index ) const -> const VkImage&;
  [[nodiscard]] auto GetFormat() const -> VkFormat;
  [[nodiscard]] auto GetExtent() const -> VkExtent2D;
  [[nodiscard]] auto GetImageCount() const -> uint32_t;

  auto Cleanup() -> void override;
};

}  // namespace EA::Rendering
