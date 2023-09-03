#pragma once

#include <eavulkan/common/includes.hpp>

namespace Rendering {

class Instance {
  VkInstance _instance{ nullptr };
  VkDebugUtilsMessengerEXT _debugMessenger{ nullptr };

  bool _isValidationEnabled{ false };
  std::vector<const char*> _enabledLayerNames;
  std::vector<const char*> _enabledExtensionNames;

  PFN_vkCreateDebugUtilsMessengerEXT _createDebugUtilsMessengerExt{ nullptr };
  PFN_vkDestroyDebugUtilsMessengerEXT _destroyDebugUtilsMessengerExt{ nullptr };

  std::vector<VkLayerProperties> _instanceLayerProperties;
  std::vector<VkExtensionProperties> _instanceExtensionProperties;
  VkDebugUtilsMessengerCreateInfoEXT _debugMessengerCreateInfo;
  VkApplicationInfo _applicationInfo;

  auto initInstanceLayerProperties() -> void;
  auto initInstanceExtensionProperties() -> void;
  auto initDebugUtilsMessengerFunctionPointers() -> void;

  auto enableInstanceLayers() -> void;
  auto enableInstanceExtension( const char* extension_name ) -> void;

 public:
  explicit Instance( std::vector<const char*> sdl_extension_names, VkApplicationInfo application_info,
                     bool enable_validation_layers = false );

  ~Instance();
  Instance( const Instance& ) = delete;
  Instance( const Instance&& ) = delete;
  auto operator=( const Instance& ) -> Instance& = delete;
  auto operator=( const Instance&& ) -> Instance& = delete;

  [[nodiscard]] auto GetHandle() const -> VkInstance;
};

}  // namespace Rendering