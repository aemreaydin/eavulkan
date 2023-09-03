#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

struct PhysicalDeviceProperties {
  VkPhysicalDeviceProperties physicalDeviceProperties{};
  VkPhysicalDeviceFeatures physicalDeviceFeatures{};
  std::vector<VkQueueFamilyProperties> queueFamilyProperties{};
  VkPhysicalDeviceMemoryProperties memoryProperties{};

  explicit PhysicalDeviceProperties(VkPhysicalDevice physical_device);
};

class Instance;
class PhysicalDevice {
  VkPhysicalDevice _physicalDevice{nullptr};
  PhysicalDeviceProperties _physicalDeviceProperties;

  static auto FindSuitablePhysicalDevice(VkInstance instance, VkSurfaceKHR surface) -> VkPhysicalDevice;

 public:
  explicit PhysicalDevice(VkInstance instance, VkSurfaceKHR surface);

  [[nodiscard]] auto GetHandle() const -> VkPhysicalDevice;
  [[nodiscard]] auto GetPhysicalDeviceProperties() const -> const PhysicalDeviceProperties&;
};

}  // namespace Rendering
