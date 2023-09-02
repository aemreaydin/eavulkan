#ifndef EAVULKAN_INSTANCE_HPP
#define EAVULKAN_INSTANCE_HPP

#include <eavulkan/common/includes.hpp>

namespace rendering {

class Instance {
  VkInstance _instance{nullptr};
  VkDebugUtilsMessengerEXT _debugMessenger{nullptr};

  bool _isValidationEnabled{false};
  std::vector<const char*> _enabledLayerNames;
  std::vector<const char*> _enabledExtensionNames;

  PFN_vkCreateDebugUtilsMessengerEXT _createDebugUtilsMessengerExt{nullptr};
  PFN_vkDestroyDebugUtilsMessengerEXT _destroyDebugUtilsMessengerExt{nullptr};

  std::vector<VkLayerProperties> _instanceLayerProperties;
  std::vector<VkExtensionProperties> _instanceExtensionProperties;
  VkDebugUtilsMessengerCreateInfoEXT _debugMessengerCreateInfo;
  VkApplicationInfo _applicationInfo;

  void initInstanceLayerProperties();
  void initInstanceExtensionProperties();
  void initDebugUtilsMessengerFunctionPointers();

  void enableInstanceLayers();
  void enableInstanceExtension(const char* extension_name);

 public:
  explicit Instance(std::vector<const char*> sdl_extension_names, VkApplicationInfo application_info,
                    bool enable_validation_layers = false);

  ~Instance();
  Instance(const Instance&) = delete;
  Instance(const Instance&&) = delete;
  auto operator=(const Instance&) -> Instance& = delete;
  auto operator=(const Instance&&) -> Instance& = delete;

  [[nodiscard]] auto GetHandle() const -> VkInstance;
};

}  // namespace rendering

#endif