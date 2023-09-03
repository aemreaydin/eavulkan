#include <eavulkan/common/common.hpp>
#include <eavulkan/physical_device.hpp>

namespace Rendering {

PhysicalDeviceProperties::PhysicalDeviceProperties(VkPhysicalDevice physical_device) {
  vkGetPhysicalDeviceProperties(physical_device, &physicalDeviceProperties);

  uint32_t queueFamilyCount{};
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, nullptr);
  queueFamilyProperties.resize(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(physical_device, &queueFamilyCount, queueFamilyProperties.data());

  vkGetPhysicalDeviceFeatures(physical_device, &physicalDeviceFeatures);
  vkGetPhysicalDeviceMemoryProperties(physical_device, &memoryProperties);
}

PhysicalDevice::PhysicalDevice(VkInstance instance, VkSurfaceKHR surface)
    : _physicalDevice(FindSuitablePhysicalDevice(instance, surface)), _physicalDeviceProperties(_physicalDevice) {}

auto PhysicalDevice::FindSuitablePhysicalDevice(VkInstance instance, VkSurfaceKHR surface) -> VkPhysicalDevice {
  uint32_t physicalDeviceCount{};
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, nullptr);
  std::vector<VkPhysicalDevice> availableDevices(physicalDeviceCount);
  vkEnumeratePhysicalDevices(instance, &physicalDeviceCount, availableDevices.data());

  if (availableDevices.empty()) {
    throw std::runtime_error("No physical devices found on the system.");
  }

  for (const auto& physicalDevice : availableDevices) {
    const auto physicalDeviceProperties = PhysicalDeviceProperties(physicalDevice);
    if (physicalDeviceProperties.physicalDeviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        surface != nullptr) {
      for (uint32_t idx = 0; idx != physicalDeviceProperties.queueFamilyProperties.size(); ++idx) {
        VkBool32 surfaceSupported{VK_FALSE};
        vkCheck(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, idx, surface, &surfaceSupported),
                "Failed to get physical device surface support.");
        if (surfaceSupported == VK_TRUE) {
          return physicalDevice;
        }
      }
    }
  }

  return availableDevices[0];
}

auto PhysicalDevice::GetHandle() const -> VkPhysicalDevice { return _physicalDevice; }

auto PhysicalDevice::GetPhysicalDeviceProperties() const -> const PhysicalDeviceProperties& {
  return _physicalDeviceProperties;
}

}  // namespace Rendering
