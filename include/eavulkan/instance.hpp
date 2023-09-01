#ifndef VInstance_h
#define VInstance_h

#include "common/common.hpp"

namespace VGraphicsEngine
{

    class VInstance
    {
        VkInstance _instance{ nullptr };
        VkDebugUtilsMessengerEXT _debugMessenger{ nullptr };

        bool _isValidationEnabled{ false };
        std::vector<const char*> _enabledLayerNames;
        std::vector<const char*> _enabledExtensionNames;

        PFN_vkCreateDebugUtilsMessengerEXT createDebugUtilsMessengerExt{ nullptr };
        PFN_vkDestroyDebugUtilsMessengerEXT destroyDebugUtilsMessengerExt{ nullptr };

        std::vector<VkLayerProperties> _instanceLayerProperties;
        std::vector<VkExtensionProperties> _instanceExtensionProperties;
        VkDebugUtilsMessengerCreateInfoEXT _debugMessengerCreateInfo;
        VkApplicationInfo _applicationInfo;

        void initInstanceLayerProperties();
        void initInstanceExtensionProperties();
        void initDebugUtilsMessengerFunctionPointers();

        void enableInstanceLayers();
        void enableInstanceExtension( const char* extensionName );

    public:
        explicit VInstance( std::vector<const char*> sdlExtensionNames, VkApplicationInfo applicationInfo,
                            bool enableValidationLayers = false );

        ~VInstance();
        VInstance( const VInstance& ) = delete;
        VInstance( const VInstance&& ) = delete;
        VInstance& operator=( const VInstance& ) = delete;
        VInstance& operator=( const VInstance&& ) = delete;

        [[nodiscard]] VkInstance getHandle() const;
    };

}  // namespace VGraphicsEngine

#endif  // VInstance_h
