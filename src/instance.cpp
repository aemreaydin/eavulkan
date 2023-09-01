#include <eavulkan/instance.hpp>

namespace VGraphicsEngine
{

    constexpr VkFlags SEVERITY_FLAGS =
            // VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    constexpr VkFlags MESSAGE_TYPE_FLAGS = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    VkBool32 VKAPI_PTR DebugUtilsMessengerCallback(
            VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT messageTypes,
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, [[maybe_unused]] void* pUserData )
{
    const auto* severity = std::invoke( [&]() {
        switch ( messageSeverity )
        {
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
                return "VERBOSE";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
                return "INFO";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
                return "WARNING";
            case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
                return "ERROR";
            default:
                throw std::runtime_error( "Invalid VkDebugUtilsMessageTypeFlagsEXT flag." );
        }
    } );

    const auto* type = std::invoke( [&]() {
        switch ( messageTypes )
        {
            case VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT:
                return "[GENERAL]";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT:
                return "[VALIDATION]";
            case VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT:
                return "[PERFORMANCE]";
            default:
                throw std::runtime_error( "Invalid VkDebugUtilsMessageTypeFlagsEXT flag." );
        }
    } );

    const auto* msgIdName =
            pCallbackData->pMessageIdName != nullptr ? pCallbackData->pMessageIdName : "NO_ID";

    fmt::print( "\n{} {}:\n({}) {}\n", severity, type, msgIdName, pCallbackData->pMessage );
    return VK_FALSE;
}

constexpr VkDebugUtilsMessengerCreateInfoEXT DEBUG_UTILS_MESSENGER_CREATE_INFO = {
        VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        nullptr,
        0,
        SEVERITY_FLAGS,
        MESSAGE_TYPE_FLAGS,
        &DebugUtilsMessengerCallback
};

constexpr std::array<const char*, 1> VALIDATION_LAYERS{
        "VK_LAYER_KHRONOS_validation",
        //    "VK_LAYER_LUNARG_api_dump"
};

VInstance::VInstance( std::vector<const char*> sdlExtensionNames, VkApplicationInfo applicationInfo,
                      bool enableValidationLayers )
        : _enabledExtensionNames( std::move( sdlExtensionNames ) ), _applicationInfo( applicationInfo ),
          _isValidationEnabled( enableValidationLayers ),
          _debugMessengerCreateInfo( DEBUG_UTILS_MESSENGER_CREATE_INFO )
{
    initInstanceLayerProperties();
    initInstanceExtensionProperties();

#if __APPLE__
    enableInstanceExtension( "VK_KHR_portability_enumeration" );
#endif

    void* pNext{ nullptr };
    if ( _isValidationEnabled )
    {
        enableInstanceLayers();
        enableInstanceExtension( "VK_EXT_debug_utils" );
#if __APPLE__
        enableInstanceExtension( "VK_KHR_get_physical_device_properties2" );
#endif
        pNext = static_cast<VkDebugUtilsMessengerCreateInfoEXT*>( &_debugMessengerCreateInfo );
    }

#if __APPLE__
    const VkInstanceCreateFlags flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR;
#else
    const VkInstanceCreateFlags flags = 0;
#endif

    const VkInstanceCreateInfo instanceCreateInfo{ VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
                                                   pNext,
                                                   flags,
                                                   &_applicationInfo,
                                                   TO_U32( _enabledLayerNames.size() ),
                                                   _enabledLayerNames.data(),
                                                   TO_U32( _enabledExtensionNames.size() ),
                                                   _enabledExtensionNames.data() };

    VK_CHECK( vkCreateInstance( &instanceCreateInfo, nullptr, &_instance ),
              "Failed to create instance." );

    if ( _isValidationEnabled )
    {
        initDebugUtilsMessengerFunctionPointers();
        VK_CHECK( createDebugUtilsMessengerExt( _instance, &_debugMessengerCreateInfo, nullptr,
                                                &_debugMessenger ),
                  "Failed to create debug messenger" );
    }
}

VInstance::~VInstance()
{
    if ( _isValidationEnabled )
    {
        destroyDebugUtilsMessengerExt( _instance, _debugMessenger, nullptr );
    }
    vkDestroyInstance( _instance, nullptr );
}

void VInstance::initInstanceLayerProperties()
{
    uint32_t layerCount{};
    vkEnumerateInstanceLayerProperties( &layerCount, nullptr );
    _instanceLayerProperties.resize( layerCount );
    vkEnumerateInstanceLayerProperties( &layerCount, _instanceLayerProperties.data() );
}

void VInstance::initInstanceExtensionProperties()
{
    uint32_t extCount{};
    vkEnumerateInstanceExtensionProperties( nullptr, &extCount, nullptr );
    _instanceExtensionProperties.resize( extCount );
    vkEnumerateInstanceExtensionProperties( nullptr, &extCount, _instanceExtensionProperties.data() );
}

void VInstance::initDebugUtilsMessengerFunctionPointers()
{
    createDebugUtilsMessengerExt = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr( _instance, "vkCreateDebugUtilsMessengerEXT" ) );
    destroyDebugUtilsMessengerExt = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(
            vkGetInstanceProcAddr( _instance, "vkDestroyDebugUtilsMessengerEXT" ) );
}

void VInstance::enableInstanceLayers()
{
    for ( const auto& property : _instanceLayerProperties )
    {
        for ( const auto& validationLayerName : VALIDATION_LAYERS )
        {
            if ( strcmp( validationLayerName, std::span( property.layerName ).data() ) == 0 )
            {
                _enabledLayerNames.push_back( validationLayerName );
            }
        }
    }
}

void VInstance::enableInstanceExtension( const char* extensionName )
{
    for ( const auto& extensionProperties : _instanceExtensionProperties )
    {
        if ( strcmp( extensionName, std::span( extensionProperties.extensionName ).data() ) == 0 )
        {
            _enabledExtensionNames.push_back( extensionName );
            return;
        }
    }
    spdlog::warn( "Failed to enabled extension {}", extensionName );
}

VkInstance VInstance::getHandle() const
{
    return _instance;
}

}  // namespace VGraphicsEngine
