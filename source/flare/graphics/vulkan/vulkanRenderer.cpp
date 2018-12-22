/*
* MIT License
*
* Copyright(c) 2018 Jimmie Bergmann
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files(the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include "flare/graphics/vulkan/vulkanRenderer.hpp"

#if defined(FLARE_VULKAN)

#include "flare/graphics/vulkan/vulkanTexture.hpp"
#include "flare/graphics/pipeline.hpp"
#include <iostream>
#include <map>
#include <algorithm>
#include <fstream>

#define FLARE_MAX_FRAMES_IN_FLIGHT 2

#define CHECK_LOADED \
    if(!m_loaded) { throw std::runtime_error("Renderer has not been loaded."); }\

#define LAMBA_ADD_TO_CLEANER \
    [this](auto ptr) { m_cleaner.add(ptr); }

static const std::vector<const char *> g_validationLayers =
{
    "VK_LAYER_LUNARG_standard_validation"
};

static const std::vector<const char *> g_deviceExtensions =
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

static VkResult vulkanCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
static VkResult vulkanDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
static std::vector<char> readFile(const std::string & filename);

namespace Flare
{

    VulkanRenderer::VulkanRenderer() :
        m_instance(0),
        m_callback(0),
        m_surface(0),
        m_graphicDevice(),
        m_graphicQueue(0),
        m_presentQueue(0),
        m_swapChain(0),
        m_renderPass(0),
        m_pipelineLayout(0),
        m_graphicsPipeline(0),
        m_commandPool(0),
        m_currentFrame(0),
        m_loaded(false)
    {
    }

    VulkanRenderer::VulkanRenderer(const RendererSettings & settings) :
        VulkanRenderer()
    {
        load(settings);
    }

    VulkanRenderer::~VulkanRenderer()
    {
        unload();
        
    }

    void VulkanRenderer::load(const RendererSettings & settings)
    {
        if (m_loaded)
        {
            unload();
        }

        m_settings = settings;

        loadCreateInstance();
        loadSetupDebugCallback();
        loadCreateSurface();
        loadPickPhysicalDevice();
        loadCreateLogicalDevice();
        loadCreateSwapChain();
        loadCreateImageViews();
        loadCreateRenderPass();
        loadCreateGraphicsPipeline();
        loadCreateFramebuffers();
        loadCreateCommandPool();
        loadCreateCommandBuffers();
        loadCreateSyncObjects();

        m_cleaner.start(0.01f, 0.01f);
        m_loaded = true;
    }

    void VulkanRenderer::unload()
    {
        m_cleaner.stop();


        unloadSwapChain();

        if (m_graphicDevice.logicalDevice)
        {
            for (auto & sem : m_renderFinishedSemaphores)
            {
                vkDestroySemaphore(m_graphicDevice.logicalDevice, sem, nullptr);
            }
            for (auto & sem : m_imageAvailableSemaphores)
            {
                vkDestroySemaphore(m_graphicDevice.logicalDevice, sem, nullptr);
            }
            for (auto & fence : m_inFlightFences)
            {
                vkDestroyFence(m_graphicDevice.logicalDevice, fence, nullptr);
            }
            m_renderFinishedSemaphores.clear();
            m_imageAvailableSemaphores.clear();
            m_inFlightFences.clear();

            if (m_commandPool)
            {
                vkDestroyCommandPool(m_graphicDevice.logicalDevice, m_commandPool, nullptr);
                m_commandPool = 0;
            }

            vkDestroyDevice(m_graphicDevice.logicalDevice, nullptr);
            m_graphicDevice.logicalDevice = 0;
        }

        if (m_instance)
        {
            if (m_callback)
            {
                vulkanDestroyDebugUtilsMessengerEXT(m_instance, m_callback, nullptr);
                m_callback = 0;
            }

            if (m_surface)
            {
                vkDestroySurfaceKHR(m_instance, m_surface, nullptr);
                m_surface = 0;
            }
            
            vkDestroyInstance(m_instance, nullptr);
            m_instance = 0;
        }
    }

    void VulkanRenderer::update()
    {
    }

    void VulkanRenderer::render()
    {
        loadDrawFrame();
        vkDeviceWaitIdle(m_graphicDevice.logicalDevice);
    }

    void VulkanRenderer::resize(const Vector2ui32 & size)
    {

    }

    const RenderMemoryAllocator & VulkanRenderer::memory() const
    {
        return m_memory;
    }

    std::shared_ptr<Texture> VulkanRenderer::createTexture()
    {
        CHECK_LOADED;

        auto texture = new VulkanTexture(m_memory);
        auto ptr = std::shared_ptr<VulkanTexture>(texture, LAMBA_ADD_TO_CLEANER);
        return ptr;
    }

    std::shared_ptr<Pipeline> VulkanRenderer::createPipeline()
    {
        CHECK_LOADED;

        std::shared_ptr<Pipeline> ptr;
        //auto ptr = std::shared_ptr<Pipeline>(pipeline, LAMBA_ADD_TO_CLEANER);

        return ptr;
    }

    VulkanRenderer::VulkanGraphicDevice::VulkanGraphicDevice() :
        physicalDevice(nullptr),
        logicalDevice(nullptr),
        hasDeviceExtensionSupport(false)
    { }

    bool VulkanRenderer::VulkanGraphicDevice::isSuitable() const
    {
        return graphicsFamily.has_value() && presentFamily.has_value() && hasDeviceExtensionSupport &&
               !surfaceFormats.empty() && !presentModes.empty();
    }

#if defined(FLARE_PLATFORM_WINDOWS)
    void VulkanRenderer::getHWndHInstance(HWND & hWnd, HINSTANCE & hInstance)
    {
        hWnd = 0;
        hInstance = 0;

        const Window * pWindow = m_settings.getWindow();
        const WindowProxy & windowProxy = m_settings.getWindowProxy();

        if (pWindow)
        {
            hWnd = pWindow->getHWnd();
            hInstance = pWindow->getHInstance();
        }
        if (!hWnd)
        {
            hWnd = windowProxy.getHWnd();
        }
        if (!hInstance)
        {
            hInstance = windowProxy.getHInstance();
        }
    }
#endif

    // Vulkan loading functions.
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanRenderer::staticDebugCallback(
        VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
        VkDebugUtilsMessageTypeFlagsEXT messageType,
        const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
        void* pUserData)
    {
        if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT)
        {
            std::cout << "validation layer - verbose: " << pCallbackData->pMessage << std::endl;
        }
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            std::cout << "validation layer - warning: " << pCallbackData->pMessage << std::endl;
        }
        else if (messageSeverity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
        {
            std::cerr << "validation layer - error: " << pCallbackData->pMessage << std::endl;
        }
        else
        {
            std::cout << "validation layer - other: " << pCallbackData->pMessage << std::endl;
        }

        return VK_FALSE;
    }

    bool VulkanRenderer::loadCheckValidationLayerSupport()
    {
        uint32_t layerCount = 0;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : g_validationLayers)
        {
            bool layerFound = false;

            for (const auto & layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }
        }

        return true;
    }

    void VulkanRenderer::loadGetRequiredExtensions(std::vector<const char*> & extensions)
    {
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, NULL);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(NULL, &extensionCount, availableExtensions.data());

        for (auto & extension : availableExtensions)
        {
            if (strcmp("VK_KHR_surface", extension.extensionName) == 0)
            {
                extensions.push_back("VK_KHR_surface");
            }
            #if defined(FLARE_PLATFORM_WINDOWS)
            else if( strcmp("VK_KHR_win32_surface", extension.extensionName) == 0)
            {
                extensions.push_back("VK_KHR_win32_surface");
            }
            #endif
        }

        if (m_settings.getDebug())
        {
            extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }
    }

    void VulkanRenderer::loadCreateInstance()
    {
        const bool enableValidationLayers = m_settings.getDebug();
        if (enableValidationLayers && !loadCheckValidationLayerSupport())
        {
            throw std::runtime_error("Validation layers requested(debug), but not available.");
        }

        // App info.
        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Vulkan test";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "Flare";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_1;

        // Instance create info.
        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        // Set extensions
        std::vector<const char*> extensions;
        loadGetRequiredExtensions(extensions);
        createInfo.enabledExtensionCount = static_cast<uint32_t>(extensions.size());
        createInfo.ppEnabledExtensionNames = extensions.data();

        // Set validation layers.
        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
            createInfo.ppEnabledLayerNames = g_validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        if (vkCreateInstance(&createInfo, nullptr, &m_instance) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create instance.");
        }
    }

    void VulkanRenderer::loadSetupDebugCallback()
    {
        if (!m_settings.getDebug())
        {
            return;
        }

        VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
        createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
            VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        createInfo.pfnUserCallback = staticDebugCallback;
        createInfo.pUserData = nullptr;

        if (vulkanCreateDebugUtilsMessengerEXT(m_instance, &createInfo, nullptr, &m_callback) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to setup debug callback.");
        }
    }

    void VulkanRenderer::loadCreateSurface()
    {
#if defined(FLARE_PLATFORM_WINDOWS)
        HWND hWnd;
        HINSTANCE hInstance;
        getHWndHInstance(hWnd, hInstance);

        if (hWnd == nullptr)
        {
            throw std::runtime_error("Cannot retreive window handle.");
        }

        VkWin32SurfaceCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = hWnd;
        createInfo.hinstance = hInstance;//GetModuleHandle(nullptr);

        auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)vkGetInstanceProcAddr(m_instance, "vkCreateWin32SurfaceKHR");
        if (!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(m_instance, &createInfo, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create window surface.");
        }
#endif
    }

    void VulkanRenderer::loadQuerySwapChainSupport(VulkanGraphicDevice & graphicDevice, VkPhysicalDevice physicalDevice)
    {
        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, m_surface, &graphicDevice.surfaceCapabilities);

        uint32_t formatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, nullptr);
        if (formatCount != 0)
        {
            graphicDevice.surfaceFormats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, m_surface, &formatCount, graphicDevice.surfaceFormats.data());
        }

        uint32_t presentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, nullptr);
        if (presentModeCount != 0)
        {
            graphicDevice.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, m_surface, &presentModeCount, graphicDevice.presentModes.data());
        }
    }

    void VulkanRenderer::loadGraphicDevice(VulkanGraphicDevice & graphicDevice, VkPhysicalDevice physicalDevice)
    {
        graphicDevice.physicalDevice = physicalDevice;

        // Check queue family.
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

        uint32_t index = 0;
        for (const auto & queueFamily : queueFamilies)
        {
            if (queueFamily.queueCount > 0)
            {
                if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
                {
                    graphicDevice.graphicsFamily = index;
                }

                VkBool32 presentSupport = false;
                vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, m_surface, &presentSupport);
                if (presentSupport)
                {
                    graphicDevice.presentFamily = index;
                }

                if (graphicDevice.graphicsFamily.has_value() && graphicDevice.presentFamily.has_value())
                {
                    break;
                }
            }

            index++;
        }


        // Check device extension support
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(g_deviceExtensions.begin(), g_deviceExtensions.end());
        for (const auto & extension : availableExtensions)
        {
            requiredExtensions.erase(extension.extensionName);
        }
        graphicDevice.hasDeviceExtensionSupport = requiredExtensions.empty();
      

        // Query swapchain.
        loadQuerySwapChainSupport(graphicDevice, physicalDevice);
    }

    void VulkanRenderer::loadScorePhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t & score)
    {
        score = 0;

        // Get device properties. (device type, limits, etc).
        VkPhysicalDeviceProperties properties;
        vkGetPhysicalDeviceProperties(physicalDevice, &properties);

        // Device memory properties.
        VkPhysicalDeviceMemoryProperties memoryProperties;
        vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

        // Check for dedicated GPU.
        if (properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
        {
            score += 1000;
        }

        // Check for dedicated VRAM.
        for (size_t i = 0; i < memoryProperties.memoryHeapCount; i++)
        {
            if (memoryProperties.memoryHeaps[i].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT)
            {
                score += static_cast<uint32_t>(memoryProperties.memoryHeaps[i].size / 1000000000ULL);
            }
            
        }
    }

    void VulkanRenderer::loadPickPhysicalDevice()
    {
        uint32_t physicalDeviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, nullptr);
        if (physicalDeviceCount == 0)
        {
            throw std::runtime_error("Failed to find GPUs with Vulkan support!");
        }
        std::vector<VkPhysicalDevice> physicalDevices(physicalDeviceCount);
        vkEnumeratePhysicalDevices(m_instance, &physicalDeviceCount, physicalDevices.data());

        std::multimap<uint32_t, VulkanGraphicDevice> scoredDevices;
        for (const auto & physicalDevice : physicalDevices)
        {
            VulkanGraphicDevice newDevice;
            loadGraphicDevice(newDevice, physicalDevice);

            if (!newDevice.isSuitable())
            {
                continue;
            }

            uint32_t score = 0;
            loadScorePhysicalDevice(physicalDevice, score);

            scoredDevices.insert({ score, newDevice});
        }

        if (scoredDevices.size())
        {
            m_graphicDevice = scoredDevices.rbegin()->second;
        }
        else
        {
            throw std::runtime_error("Failed to find suitable GPU.");
        }
    }

    void VulkanRenderer::loadCreateLogicalDevice()
    {
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { m_graphicDevice.graphicsFamily.value(), m_graphicDevice.presentFamily.has_value() };

        float queuePriority = 1.0f;
        for (uint32_t queueFamily : uniqueQueueFamilies)
        {
            VkDeviceQueueCreateInfo queueCreateInfo = {};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkPhysicalDeviceFeatures deviceFeatures = {};
        VkDeviceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();
        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = static_cast<uint32_t>(g_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = g_deviceExtensions.data();

        // Set validation layers.
        if (m_settings.getDebug())
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(g_validationLayers.size());
            createInfo.ppEnabledLayerNames = g_validationLayers.data();
        }
        else
        {
            createInfo.enabledLayerCount = 0;
        }

        // Create device.
        if (vkCreateDevice(m_graphicDevice.physicalDevice, &createInfo, nullptr, &m_graphicDevice.logicalDevice) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create logical device.");
        }

        vkGetDeviceQueue(m_graphicDevice.logicalDevice, m_graphicDevice.graphicsFamily.value(), 0, &m_graphicQueue);
        vkGetDeviceQueue(m_graphicDevice.logicalDevice, m_graphicDevice.presentFamily.value(), 0, &m_presentQueue);
    }

    void VulkanRenderer::loadChooseSwapSurfaceFormat()
    {
        if (m_graphicDevice.surfaceFormats.size() == 1 && m_graphicDevice.surfaceFormats[0].format == VK_FORMAT_UNDEFINED)
        {
            m_swapChainSurfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLORSPACE_SRGB_NONLINEAR_KHR };
        }

        for (const auto & format : m_graphicDevice.surfaceFormats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_UNORM && format.colorSpace == VK_COLORSPACE_SRGB_NONLINEAR_KHR)
            {
                m_swapChainSurfaceFormat = format;
                return;
            }
        }

        throw std::runtime_error("Failed to choose swap surface format.");
    }

    void VulkanRenderer::loadChooseSwapPresentMode()
    {
        m_swapChainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

        for (const auto & mode : m_graphicDevice.presentModes)
        {
            if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
            {
                m_swapChainPresentMode = mode;
                return;
            }
            else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
            {
                m_swapChainPresentMode = mode;
            }
        }
    }

    void VulkanRenderer::loadChooseSwapExtent()
    {
        if (m_graphicDevice.surfaceCapabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
        {
            m_swapChainExtent = m_graphicDevice.surfaceCapabilities.currentExtent;
        }
        else
        {
            m_swapChainExtent = { 0,0 };
            const Window * window = m_settings.getWindow();
            if (window)
            {
                const auto & windowSize = window->getSize();
                m_swapChainExtent.width = windowSize.x;
                m_swapChainExtent.height = windowSize.y;
            }
            m_swapChainExtent.width = std::max(m_graphicDevice.surfaceCapabilities.minImageExtent.width, std::min(m_graphicDevice.surfaceCapabilities.maxImageExtent.width, m_swapChainExtent.width));
            m_swapChainExtent.height = std::max(m_graphicDevice.surfaceCapabilities.minImageExtent.height, std::min(m_graphicDevice.surfaceCapabilities.maxImageExtent.height, m_swapChainExtent.height));
        }
       
    }

    void VulkanRenderer::loadCreateSwapChain()
    {
        loadQuerySwapChainSupport(m_graphicDevice, m_graphicDevice.physicalDevice);

        loadChooseSwapSurfaceFormat();
        loadChooseSwapPresentMode();
        loadChooseSwapExtent();

        uint32_t imageCount = m_graphicDevice.surfaceCapabilities.minImageCount + 1;
        if (m_graphicDevice.surfaceCapabilities.maxImageCount > 0 && imageCount > m_graphicDevice.surfaceCapabilities.maxImageCount)
        {
            imageCount = m_graphicDevice.surfaceCapabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface = m_surface;
        createInfo.minImageCount = imageCount;
        createInfo.imageFormat = m_swapChainSurfaceFormat.format;
        createInfo.imageColorSpace = m_swapChainSurfaceFormat.colorSpace;
        createInfo.imageExtent = m_swapChainExtent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { m_graphicDevice.graphicsFamily.value(), m_graphicDevice.presentFamily.value() };

        if (m_graphicDevice.graphicsFamily != m_graphicDevice.presentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices;
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        createInfo.preTransform = m_graphicDevice.surfaceCapabilities.currentTransform;
        createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode = m_swapChainPresentMode;
        createInfo.clipped = VK_TRUE;
        createInfo.oldSwapchain = VK_NULL_HANDLE;

        if (vkCreateSwapchainKHR(m_graphicDevice.logicalDevice, &createInfo, nullptr, &m_swapChain) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create swap chain.");
        }

        vkGetSwapchainImagesKHR(m_graphicDevice.logicalDevice, m_swapChain, &imageCount, nullptr);
        m_swapChainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_graphicDevice.logicalDevice, m_swapChain, &imageCount, m_swapChainImages.data());
    }

    void VulkanRenderer::loadCreateImageViews()
    {
        m_swapChainImageViews.resize(m_swapChainImages.size());

        for (size_t i = 0; i < m_swapChainImages.size(); i++)
        {
            VkImageViewCreateInfo createInfo = {};
            createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            createInfo.image = m_swapChainImages[i];
            createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
            createInfo.format = m_swapChainSurfaceFormat.format;
            createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
            createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            createInfo.subresourceRange.baseMipLevel = 0;
            createInfo.subresourceRange.levelCount = 1;
            createInfo.subresourceRange.baseArrayLayer = 0;
            createInfo.subresourceRange.layerCount = 1;

            if (vkCreateImageView(m_graphicDevice.logicalDevice, &createInfo, nullptr, &m_swapChainImageViews[i]) != VK_SUCCESS)
            {
                throw std::runtime_error(std::string("Failed to create image view ") +
                    std::to_string(static_cast<int>(i)).c_str());
            }
        }
    }

    void VulkanRenderer::loadCreateRenderPass()
    {
        VkAttachmentDescription colorAttachment = {};
        colorAttachment.format = m_swapChainSurfaceFormat.format;
        colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
        colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

        VkAttachmentReference colorAttachmentRef = {};
        colorAttachmentRef.attachment = 0;
        colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

        VkSubpassDescription subpass = {};
        subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        subpass.colorAttachmentCount = 1;
        subpass.pColorAttachments = &colorAttachmentRef;


        VkSubpassDependency dependency = {};
        dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
        dependency.dstSubpass = 0;
        dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.srcAccessMask = 0;
        dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;


        VkRenderPassCreateInfo renderPassInfo = {};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
        renderPassInfo.attachmentCount = 1;
        renderPassInfo.pAttachments = &colorAttachment;
        renderPassInfo.subpassCount = 1;
        renderPassInfo.pSubpasses = &subpass;
        renderPassInfo.dependencyCount = 1;
        renderPassInfo.pDependencies = &dependency;

        if (vkCreateRenderPass(m_graphicDevice.logicalDevice, &renderPassInfo, nullptr, &m_renderPass) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create render pass.");
        }
    }

    void VulkanRenderer::loadCreateGraphicsPipeline()
    {
        auto vertShaderCode = readFile("shaders/vert.spv");
        auto fragShaderCode = readFile("shaders/frag.spv");

        VkShaderModule vertShaderModule; 
        VkShaderModule fragShaderModule; 
        loadCreateShaderModule(vertShaderCode, vertShaderModule);
        loadCreateShaderModule(fragShaderCode, fragShaderModule);

        VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};
        vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
        vertShaderStageInfo.module = vertShaderModule;
        vertShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};
        fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
        fragShaderStageInfo.module = fragShaderModule;
        fragShaderStageInfo.pName = "main";

        VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

        VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};
        vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputInfo.vertexBindingDescriptionCount = 0;
        vertexInputInfo.pVertexBindingDescriptions = nullptr;
        vertexInputInfo.vertexAttributeDescriptionCount = 0;
        vertexInputInfo.pVertexAttributeDescriptions = nullptr;

        VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
        inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
        inputAssembly.primitiveRestartEnable = VK_FALSE;

        VkViewport viewport = {};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(m_swapChainExtent.width);
        viewport.height = static_cast<float>(m_swapChainExtent.height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;

        VkRect2D scissor = {};
        scissor.offset = { 0, 0 };
        scissor.extent = m_swapChainExtent;

        VkPipelineViewportStateCreateInfo viewportState = {};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.pViewports = &viewport;
        viewportState.scissorCount = 1;
        viewportState.pScissors = &scissor;

        VkPipelineRasterizationStateCreateInfo rasterizer = {};
        rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizer.depthClampEnable = VK_FALSE;
        rasterizer.rasterizerDiscardEnable = VK_FALSE;
        rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
        rasterizer.lineWidth = 1.0f;
        rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
        rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
        rasterizer.depthBiasEnable = VK_FALSE;
        rasterizer.depthBiasConstantFactor = 0.0f;
        rasterizer.depthBiasClamp = 0.0f;
        rasterizer.depthBiasSlopeFactor = 0.0f;

        VkPipelineMultisampleStateCreateInfo multisampling = {};
        multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multisampling.sampleShadingEnable = VK_FALSE;
        multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

        VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
        colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT
            | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
        colorBlendAttachment.blendEnable = VK_FALSE;
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

        VkPipelineColorBlendStateCreateInfo colorBlend = {};
        colorBlend.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
        colorBlend.logicOpEnable = VK_FALSE;
        colorBlend.logicOp = VK_LOGIC_OP_COPY;
        colorBlend.attachmentCount = 1;
        colorBlend.pAttachments = &colorBlendAttachment;
        colorBlend.blendConstants[0] = 0.0f;
        colorBlend.blendConstants[1] = 0.0f;
        colorBlend.blendConstants[2] = 0.0f;
        colorBlend.blendConstants[3] = 0.0f;

        VkPipelineLayoutCreateInfo pipelineLayoutInfo = {};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 0;
        pipelineLayoutInfo.pPushConstantRanges = nullptr;

        if (vkCreatePipelineLayout(m_graphicDevice.logicalDevice, &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create pipeline layout.");
        }

        VkGraphicsPipelineCreateInfo pipelineInfo = {};
        pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
        pipelineInfo.stageCount = 2;
        pipelineInfo.pStages = shaderStages;
        pipelineInfo.pVertexInputState = &vertexInputInfo;
        pipelineInfo.pInputAssemblyState = &inputAssembly;
        pipelineInfo.pViewportState = &viewportState;
        pipelineInfo.pRasterizationState = &rasterizer;
        pipelineInfo.pMultisampleState = &multisampling;
        pipelineInfo.pDepthStencilState = nullptr;
        pipelineInfo.pColorBlendState = &colorBlend;
        pipelineInfo.pDynamicState = nullptr;
        pipelineInfo.layout = m_pipelineLayout;
        pipelineInfo.renderPass = m_renderPass;
        pipelineInfo.subpass = 0;
        pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
        pipelineInfo.basePipelineIndex = -1;

        if (vkCreateGraphicsPipelines(m_graphicDevice.logicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &m_graphicsPipeline) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create graphics pipeline.");
        }

        vkDestroyShaderModule(m_graphicDevice.logicalDevice, fragShaderModule, nullptr);
        vkDestroyShaderModule(m_graphicDevice.logicalDevice, vertShaderModule, nullptr);
    }

    void VulkanRenderer::loadCreateShaderModule(const std::vector<char> & code, VkShaderModule & shaderModule) const
    {
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if (vkCreateShaderModule(m_graphicDevice.logicalDevice, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to create shader module.");
        }
    }

    void VulkanRenderer::loadCreateFramebuffers()
    {
        m_swapChainFramebuffers.resize(m_swapChainImageViews.size());

        for (size_t i = 0; i < m_swapChainImageViews.size(); i++)
        {
            VkImageView attachments[] =
            {
                m_swapChainImageViews[i]
            };

            VkFramebufferCreateInfo framebufferInfo = {};
            framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
            framebufferInfo.renderPass = m_renderPass;
            framebufferInfo.attachmentCount = 1;
            framebufferInfo.pAttachments = attachments;
            framebufferInfo.width = m_swapChainExtent.width;
            framebufferInfo.height = m_swapChainExtent.height;
            framebufferInfo.layers = 1;

            if (vkCreateFramebuffer(m_graphicDevice.logicalDevice, &framebufferInfo, nullptr, &m_swapChainFramebuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create framebuffer.");
            }
        }
    }

    void VulkanRenderer::loadCreateCommandPool()
    {
        VkCommandPoolCreateInfo poolInfo = {};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex = m_graphicDevice.graphicsFamily.value();
        poolInfo.flags = 0;

        if (vkCreateCommandPool(m_graphicDevice.logicalDevice, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("Faild to create command pool.");
        }
    }

    void VulkanRenderer::loadCreateCommandBuffers()
    {
        m_commandBuffers.resize(m_swapChainFramebuffers.size());

        VkCommandBufferAllocateInfo allocInfo = {};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
        if (vkAllocateCommandBuffers(m_graphicDevice.logicalDevice, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to allocate command buffers.");
        }

        for (size_t i = 0; i < m_commandBuffers.size(); i++)
        {
            VkCommandBufferBeginInfo beginInfo = {};
            beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
            beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
            beginInfo.pInheritanceInfo = nullptr;

            if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to begin recording of command buffer.");
            }

            VkRenderPassBeginInfo renderPassInfo = {};
            renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
            renderPassInfo.renderPass = m_renderPass;
            renderPassInfo.framebuffer = m_swapChainFramebuffers[i];
            renderPassInfo.renderArea.offset = { 0, 0 };
            renderPassInfo.renderArea.extent = m_swapChainExtent;

            VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
            renderPassInfo.clearValueCount = 1;
            renderPassInfo.pClearValues = &clearColor;

            vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
            vkCmdBindPipeline(m_commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, m_graphicsPipeline);
            vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);
            vkCmdEndRenderPass(m_commandBuffers[i]);

            if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to record command buffer.");
            }
        }
    }

    void VulkanRenderer::loadDrawFrame()
    {
        vkWaitForFences(m_graphicDevice.logicalDevice, 1, &m_inFlightFences[m_currentFrame], VK_TRUE, std::numeric_limits<uint64_t>::max());
        vkResetFences(m_graphicDevice.logicalDevice, 1, &m_inFlightFences[m_currentFrame]);

        uint32_t imageIndex;
        VkResult result = vkAcquireNextImageKHR(m_graphicDevice.logicalDevice, m_swapChain, std::numeric_limits<uint64_t>::max(),
            m_imageAvailableSemaphores[m_currentFrame], VK_NULL_HANDLE, &imageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR) {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }



        VkSubmitInfo submitInfo = {};
        submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

        VkSemaphore waitSemaphore[] = { m_imageAvailableSemaphores[m_currentFrame] };
        VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
        submitInfo.waitSemaphoreCount = 1;
        submitInfo.pWaitSemaphores = waitSemaphore;
        submitInfo.pWaitDstStageMask = waitStages;
        submitInfo.commandBufferCount = 1;
        submitInfo.pCommandBuffers = &m_commandBuffers[imageIndex];
        VkSemaphore signalSemaphores[] = { m_renderFinishedSemaphores[m_currentFrame] };
        submitInfo.signalSemaphoreCount = 1;
        submitInfo.pSignalSemaphores = signalSemaphores;

        if (vkQueueSubmit(m_graphicQueue, 1, &submitInfo, m_inFlightFences[m_currentFrame]) != VK_SUCCESS)
        {
            throw std::runtime_error("Failed to submit draw command buffer!");
        }

        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.waitSemaphoreCount = 1;
        presentInfo.pWaitSemaphores = signalSemaphores;

        VkSwapchainKHR swapChains[] = { m_swapChain };
        presentInfo.swapchainCount = 1;
        presentInfo.pSwapchains = swapChains;
        presentInfo.pImageIndices = &imageIndex;
        presentInfo.pResults = nullptr;

        result = vkQueuePresentKHR(m_presentQueue, &presentInfo);
        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return;
        }
        else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
            throw std::runtime_error("failed to acquire swap chain image!");
        }


        m_currentFrame = (m_currentFrame + 1) % FLARE_MAX_FRAMES_IN_FLIGHT;

    }

    void VulkanRenderer::loadCreateSyncObjects()
    {
        m_imageAvailableSemaphores.resize(FLARE_MAX_FRAMES_IN_FLIGHT);
        m_renderFinishedSemaphores.resize(FLARE_MAX_FRAMES_IN_FLIGHT);
        m_inFlightFences.resize(FLARE_MAX_FRAMES_IN_FLIGHT);

        VkSemaphoreCreateInfo semaphoreInfo = {};
        semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fenceInfo = {};
        fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

        for (size_t i = 0; i < FLARE_MAX_FRAMES_IN_FLIGHT; i++)
        {
            if (vkCreateSemaphore(m_graphicDevice.logicalDevice, &semaphoreInfo, nullptr, &m_imageAvailableSemaphores[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_graphicDevice.logicalDevice, &semaphoreInfo, nullptr, &m_renderFinishedSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_graphicDevice.logicalDevice, &fenceInfo, nullptr, &m_inFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("Failed to create synchronization objects for a frame.");
            }
        }
    }

    void VulkanRenderer::unloadSwapChain()
    {
        if (m_graphicDevice.logicalDevice)
        {
            for (auto framebuffer : m_swapChainFramebuffers)
            {
                vkDestroyFramebuffer(m_graphicDevice.logicalDevice, framebuffer, nullptr);
            }
            m_swapChainFramebuffers.clear();

            vkFreeCommandBuffers(m_graphicDevice.logicalDevice, m_commandPool, static_cast<uint32_t>(m_commandBuffers.size()), m_commandBuffers.data());
            m_commandBuffers.clear();

            if (m_graphicsPipeline)
            {
                vkDestroyPipeline(m_graphicDevice.logicalDevice, m_graphicsPipeline, nullptr);
                m_graphicsPipeline = 0;
            }

            if (m_pipelineLayout)
            {
                vkDestroyPipelineLayout(m_graphicDevice.logicalDevice, m_pipelineLayout, nullptr);
                m_pipelineLayout = 0;
            }

            if (m_renderPass)
            {
                vkDestroyRenderPass(m_graphicDevice.logicalDevice, m_renderPass, nullptr);
                m_renderPass = 0;
            }

            for (auto & imageView : m_swapChainImageViews)
            {
                vkDestroyImageView(m_graphicDevice.logicalDevice, imageView, nullptr);
            }
            m_swapChainImageViews.clear();

            if (m_swapChain)
            {
                vkDestroySwapchainKHR(m_graphicDevice.logicalDevice, m_swapChain, nullptr);
                m_swapChain = 0;
            }
        }

    }

    void VulkanRenderer::recreateSwapChain() {
        /*int width = 0, height = 0;
        while (width == 0 || height == 0) {
            glfwGetFramebufferSize(window, &width, &height);
            glfwWaitEvents();
        }*/

        vkDeviceWaitIdle(m_graphicDevice.logicalDevice);

        unloadSwapChain();

        loadCreateSwapChain();
        loadCreateImageViews();
        loadCreateRenderPass();
        loadCreateGraphicsPipeline();
        loadCreateFramebuffers();
        loadCreateCommandBuffers();
    }

}


// Static functions.
VkResult vulkanCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

VkResult vulkanDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator)
{
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr)
    {
        func(instance, callback, pAllocator);
        return VK_SUCCESS;
    }

    return VK_ERROR_EXTENSION_NOT_PRESENT;
}

std::vector<char> readFile(const std::string & filename)
{
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        throw std::runtime_error(("Failed to open shader: " + filename).c_str());
    }

    size_t fileSize = static_cast<size_t>(file.tellg());
    file.seekg(0);

    std::vector<char> buffer(fileSize);
    file.read(buffer.data(), fileSize);

    return buffer;
}

#endif