#include <eavulkan/rendering/device.hpp>
#include <eavulkan/rendering/physical_device.hpp>
#include <eavulkan/rendering/swap_chain.hpp>
#include <eavulkan/shared/utils.hpp>

namespace EA::Rendering {

Swapchain::Swapchain( Device const& device, const VkSurfaceKHR& surface, VkExtent2D extent )
  : DeviceResource<VkSwapchainKHR>( device ),
    _surface( surface ),
    _surfaceSupportQuery( querySurfaceSupport() ),
    _surfaceFormat( useSuitableFormat() ),
    _presentMode( useSuitablePresentMode() ),
    _extent( useSuitableExtent( extent ) ) {
  _imageCount = std::max( _surfaceSupportQuery.surfaceCapabilities.minImageCount,
                          std::min( _surfaceSupportQuery.surfaceCapabilities.minImageCount + 1,
                                    _surfaceSupportQuery.surfaceCapabilities.maxImageCount ) );
  VkSwapchainCreateInfoKHR swapchainCreateInfo{ VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR, nullptr, 0 };
  swapchainCreateInfo.surface = _surface;
  swapchainCreateInfo.minImageCount = _imageCount;
  swapchainCreateInfo.imageFormat = _surfaceFormat.format;
  swapchainCreateInfo.imageColorSpace = _surfaceFormat.colorSpace;
  swapchainCreateInfo.imageExtent = _extent;
  swapchainCreateInfo.imageArrayLayers = 1;
  swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
  swapchainCreateInfo.preTransform = _surfaceSupportQuery.surfaceCapabilities.currentTransform;
  swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  swapchainCreateInfo.presentMode = _presentMode;
  swapchainCreateInfo.clipped = VK_TRUE;
  // TODO(emreaydn): Use concurrent sharing
  swapchainCreateInfo.queueFamilyIndexCount = 0;
  swapchainCreateInfo.pQueueFamilyIndices = nullptr;
  // TODO(emreaydn): Use old swapchain
  swapchainCreateInfo.oldSwapchain = nullptr;

  vkCheck( vkCreateSwapchainKHR( GetDevice().GetHandle(), &swapchainCreateInfo, nullptr, &GetHandle() ),
           "Failed to create swapchain" );

  uint32_t count = 0;
  vkGetSwapchainImagesKHR( GetDevice().GetHandle(), GetHandle(), &count, nullptr );
  _images.resize( count );
  vkGetSwapchainImagesKHR( GetDevice().GetHandle(), GetHandle(), &count, _images.data() );

  createImageViews();
}

void Swapchain::createImageViews() {
  _imageViews.resize( _images.size() );
  for ( uint32_t ind = 0; ind != _images.size(); ++ind ) {
    VkImageViewCreateInfo imageViewCreateInfo{ VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO, nullptr, 0 };
    imageViewCreateInfo.image = _images[ind];
    imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    imageViewCreateInfo.format = _surfaceFormat.format;
    imageViewCreateInfo.components = { VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY,
                                       VK_COMPONENT_SWIZZLE_IDENTITY, VK_COMPONENT_SWIZZLE_IDENTITY };
    imageViewCreateInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCheck( vkCreateImageView( GetDevice().GetHandle(), &imageViewCreateInfo, nullptr, &_imageViews[ind] ),
             "Failed to create image view." );
  }
}

void Swapchain::Cleanup() {
  for ( VkImageView imageView : _imageViews ) {
    vkDestroyImageView( GetDevice().GetHandle(), imageView, nullptr );
  }
  vkDestroySwapchainKHR( GetDevice().GetHandle(), GetHandle(), nullptr );
}

auto Swapchain::GetImageViews() const -> const std::vector<VkImageView>& { return _imageViews; }

auto Swapchain::GetImageView( uint32_t index ) const -> const VkImageView& { return gsl::at( _imageViews, index ); }

auto Swapchain::GetImage( uint32_t index ) const -> const VkImage& { return gsl::at( _images, index ); }

auto Swapchain::GetFormat() const -> VkFormat { return _surfaceFormat.format; }

auto Swapchain::GetExtent() const -> VkExtent2D { return _extent; }

auto Swapchain::querySurfaceSupport() const -> SwapchainSurfaceSupportQuery {
  SwapchainSurfaceSupportQuery query{};
  VkPhysicalDevice physicalDevice = GetDevice().GetPhysicalDevice().GetHandle();
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR( physicalDevice, _surface, &query.surfaceCapabilities );

  uint32_t surfaceFormatCount = 0;
  vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, _surface, &surfaceFormatCount, nullptr );
  query.surfaceFormats.resize( surfaceFormatCount );
  vkGetPhysicalDeviceSurfaceFormatsKHR( physicalDevice, _surface, &surfaceFormatCount, query.surfaceFormats.data() );

  uint32_t presentModeCount = 0;
  vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, _surface, &presentModeCount, nullptr );
  query.presentModes.resize( presentModeCount );
  vkGetPhysicalDeviceSurfacePresentModesKHR( physicalDevice, _surface, &presentModeCount, query.presentModes.data() );

  return query;
}

auto Swapchain::useSuitableFormat() const -> VkSurfaceFormatKHR {
  auto found = std::find_if( _surfaceSupportQuery.surfaceFormats.begin(), _surfaceSupportQuery.surfaceFormats.end(),
                             []( const VkSurfaceFormatKHR& format ) {
                               return format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR &&
                                      format.format == VK_FORMAT_R8G8B8A8_SRGB;
                             } );
  if ( found != _surfaceSupportQuery.surfaceFormats.end() ) {
    return *found;
  }

  return _surfaceSupportQuery.surfaceFormats[0];
}

auto Swapchain::useSuitablePresentMode() const -> VkPresentModeKHR {
  auto found =
    std::find_if( _surfaceSupportQuery.presentModes.cbegin(), _surfaceSupportQuery.presentModes.cend(),
                  []( const VkPresentModeKHR& present_mode ) { return present_mode == VK_PRESENT_MODE_MAILBOX_KHR; } );
  if ( found != _surfaceSupportQuery.presentModes.end() ) {
    return *found;
  }

  return VK_PRESENT_MODE_FIFO_KHR;
}

auto Swapchain::useSuitableExtent( VkExtent2D extent ) const -> VkExtent2D {
  if ( _surfaceSupportQuery.surfaceCapabilities.currentExtent.width == UINT32_MAX ) {
    return extent;
  }
  return _surfaceSupportQuery.surfaceCapabilities.currentExtent;
}

auto Swapchain::GetImageCount() const -> uint32_t { return _imageCount; }

}  // namespace EA::Rendering
