#include <eavulkan/common/common.hpp>
#include <eavulkan/instance.hpp>

namespace rendering {

constexpr VkFlags kSeverityFlags =
    // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

constexpr VkFlags kMessageTypeFlags = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

auto VKAPI_PTR debugUtilsMessengerCallback(VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                                           VkDebugUtilsMessageTypeFlagsEXT message_types,
                                           const VkDebugUtilsMessengerCallbackDataEXT* p_callback_data,
                                           [[maybe_unused]] void* p_user_data) -> VkBool32 {
  const auto* severity = std::invoke([&]() {
    switch (message_severity) {
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
        return "VERBOSE";
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
        return "INFO";
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
        return "WARNING";
      case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
        return "ERROR";
      default:
        throw std::runtime_error("Invalid VkDebugUtilsMessageTypeFlagsEXT flag.");
    }
  });

  const auto* type = std::invoke([&]() {
    switch (message_types) {
      case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
        return "[GENERAL]";
      case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
        return "[VALIDATION]";
      case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
        return "[PERFORMANCE]";
      default:
        throw std::runtime_error("Invalid VkDebugUtilsMessageTypeFlagsEXT flag.");
    }
  });

  const auto* msgIdName = p_callback_data->pMessageIdName != nullptr ? p_callback_data->pMessageIdName : "NO_ID";

  fmt::print("\n{} {}:\n({}) {}\n", severity, type, msgIdName, p_callback_data->pMessage);
  return VK_FALSE;
}

constexpr VkDebugUtilsMessengerCreateInfoEXT kDebugUtilsMessengerCreateInfo = {
    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
    nullptr,
    0,
    kSeverityFlags,
    kMessageTypeFlags,
    &debugUtilsMessengerCallback};

constexpr std::array<const char*, 1> kValidationLayers{
    "VK_LAYER_KHRONOS_validation",
    //    "VK_LAYER_LUNARG_api_dump"
};

Instance::Instance(std::vector<const char*> sdl_extension_names, VkApplicationInfo application_info,
                   bool enable_validation_layers)
    : _isValidationEnabled(enable_validation_layers),
      _enabledExtensionNames(std::move(sdl_extension_names)),
      _debugMessengerCreateInfo(kDebugUtilsMessengerCreateInfo),
      _applicationInfo(application_info) {
  initInstanceLayerProperties();
  initInstanceExtensionProperties();

#if __APPLE__
  enableInstanceExtension("VK_KHR_portability_enumeration");
#endif

  void* pNext{nullptr};
  if (_isValidationEnabled) {
    enableInstanceLayers();
    enableInstanceExtension("VK_EXT_debug_utils");
#if __APPLE__
    enableInstanceExtension("VK_KHR_get_physical_device_properties2");
#endif
    pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>(&_debugMessengerCreateInfo);
  }

#if __APPLE__
  const VkInstanceCreateFlags flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#else
  const VkInstanceCreateFlags flags = 0;
#endif

  const VkInstanceCreateInfo instanceCreateInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                                pNext,
                                                flags,
                                                &_applicationInfo,
                                                toU32(_enabledLayerNames.size()),
                                                _enabledLayerNames.data(),
                                                toU32(_enabledExtensionNames.size()),
                                                _enabledExtensionNames.data()};

  vkCheck(vkCreateInstance(&instanceCreateInfo, nullptr, &_instance), "Failed to create instance.");

  if (_isValidationEnabled) {
    initDebugUtilsMessengerFunctionPointers();
    vkCheck(_createDebugUtilsMessengerExt(_instance, &_debugMessengerCreateInfo, nullptr, &_debugMessenger),
            "Failed to create debug messenger");
  }
}

Instance::~Instance() {
  if (_isValidationEnabled) {
    _destroyDebugUtilsMessengerExt(_instance, _debugMessenger, nullptr);
  }
  vkDestroyInstance(_instance, nullptr);
}

auto Instance::initInstanceLayerProperties() -> void {
  uint32_t layerCount{};
  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  _instanceLayerProperties.resize(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, _instanceLayerProperties.data());
}

auto Instance::initInstanceExtensionProperties() -> void {
  uint32_t extCount{};
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, nullptr);
  _instanceExtensionProperties.resize(extCount);
  vkEnumerateInstanceExtensionProperties(nullptr, &extCount, _instanceExtensionProperties.data());
}

auto Instance::initDebugUtilsMessengerFunctionPointers() -> void {
  _createDebugUtilsMessengerExt = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(_instance, "vkCreateDebugUtilsMessengerEXT"));
  _destroyDebugUtilsMessengerExt = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
      vkGetInstanceProcAddr(_instance, "vkDestroyDebugUtilsMessengerEXT"));
}

auto Instance::enableInstanceLayers() -> void {
  for (const auto& property : _instanceLayerProperties) {
    for (const auto& validationLayerName : kValidationLayers) {
      if (strcmp(validationLayerName, std::span(property.layerName).data()) == 0) {
        _enabledLayerNames.push_back(validationLayerName);
      }
    }
  }
}

auto Instance::enableInstanceExtension(const char* extension_name) -> void {
  for (const auto& extensionProperties : _instanceExtensionProperties) {
    if (strcmp(extension_name, std::span(extensionProperties.extensionName).data()) == 0) {
      _enabledExtensionNames.push_back(extension_name);
      return;
    }
  }
  spdlog::warn("Failed to enabled extension {}", extension_name);
}

auto Instance::GetHandle() const -> VkInstance { return _instance; }

}  // namespace rendering
