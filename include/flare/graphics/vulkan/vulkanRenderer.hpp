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

#ifndef FLARE_GRAPHICS_RENDERERS_VULKAN_RENDERER_HPP
#define FLARE_GRAPHICS_RENDERERS_VULKAN_RENDERER_HPP

#include "flare/graphics/renderer.hpp"

#if defined(FLARE_VULKAN)

#include "vulkan/vulkan.h"
#if defined(FLARE_PLATFORM_WINDOWS)
#include "vulkan/vulkan_win32.h"
#endif
#include "vulkanCleaner.hpp"
#include <atomic>
#include <set>
#include <vector>
#include <optional>

namespace Flare
{

    class VulkanTexture;

    class FLARE_API VulkanRenderer : public Renderer
    {

    public:

        VulkanRenderer();
        VulkanRenderer(const RendererSettings & settings);
        ~VulkanRenderer();

        virtual void load(const RendererSettings & settings);
        virtual void unload();
        virtual void update();
        virtual void render();
        virtual void resize(const Vector2ui32 & size);

        virtual const RenderMemoryAllocator & memory() const;

        virtual std::shared_ptr<Texture> createTexture();
        virtual std::shared_ptr<Pipeline> createPipeline();

    private:

        VulkanRenderer(const VulkanRenderer &) = delete;

        struct VulkanGraphicDevice
        {
            VulkanGraphicDevice();
            bool isSuitable() const;

            VkPhysicalDevice                physicalDevice;
            VkDevice                        logicalDevice;
            std::optional<uint32_t>         graphicsFamily;
            std::optional<uint32_t>         presentFamily;
            bool                            hasDeviceExtensionSupport;
            VkSurfaceCapabilitiesKHR        surfaceCapabilities;
            std::vector<VkSurfaceFormatKHR> surfaceFormats;
            std::vector<VkPresentModeKHR>   presentModes;
        };

        #if defined(FLARE_PLATFORM_WINDOWS)
        void getHWndHInstance(HWND & hWnd, HINSTANCE & hInstance);
        #endif

        // Vulkan loading methods.
        static VKAPI_ATTR VkBool32 VKAPI_CALL staticDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT, VkDebugUtilsMessageTypeFlagsEXT, const VkDebugUtilsMessengerCallbackDataEXT*, void*);
        bool loadCheckValidationLayerSupport();
        void loadGetRequiredExtensions(std::vector<const char*> & extensions);
        void loadCreateInstance();
        void loadSetupDebugCallback();
        void loadCreateSurface();
        void loadQuerySwapChainSupport(VulkanGraphicDevice & graphicDevice, VkPhysicalDevice physicalDevice);
        void loadGraphicDevice(VulkanGraphicDevice & graphicDevice, VkPhysicalDevice physicalDevice);
        void loadScorePhysicalDevice(VkPhysicalDevice physicalDevice, uint32_t & score);
        void loadPickPhysicalDevice();
        void loadCreateLogicalDevice();
        void loadChooseSwapSurfaceFormat();
        void loadChooseSwapPresentMode();
        void loadChooseSwapExtent();
        void loadCreateSwapChain();
        void loadCreateImageViews();
        void loadCreateRenderPass();
        void loadCreateGraphicsPipeline();
        void loadCreateShaderModule(const std::vector<char> & code, VkShaderModule & shaderModule) const;
        void loadCreateFramebuffers();
        void loadCreateCommandPool();
        void loadCreateCommandBuffers();
        void loadDrawFrame();
        void loadCreateSyncObjects();

        void unloadSwapChain();
        void recreateSwapChain();

        // Vulkan structures.
        VkInstance                  m_instance;
        VkDebugUtilsMessengerEXT    m_callback;
        VkSurfaceKHR                m_surface;
        VulkanGraphicDevice         m_graphicDevice;
        VkQueue                     m_graphicQueue;
        VkQueue                     m_presentQueue;
        VkSwapchainKHR              m_swapChain;
        VkExtent2D                  m_swapChainExtent;
        VkSurfaceFormatKHR          m_swapChainSurfaceFormat;
        VkPresentModeKHR            m_swapChainPresentMode;
        std::vector<VkImage>        m_swapChainImages;
        std::vector<VkImageView>    m_swapChainImageViews;
        VkRenderPass                m_renderPass;
        VkPipelineLayout            m_pipelineLayout;
        VkPipeline                  m_graphicsPipeline;
        std::vector<VkFramebuffer>  m_swapChainFramebuffers;
        VkCommandPool               m_commandPool;
        std::vector<VkCommandBuffer> m_commandBuffers;
        std::vector<VkSemaphore>    m_imageAvailableSemaphores;
        std::vector<VkSemaphore>    m_renderFinishedSemaphores;
        std::vector<VkFence>        m_inFlightFences;
        size_t                      m_currentFrame;

        RenderMemoryAllocator   m_memory;
        VulkanCleaner           m_cleaner;
        RendererSettings        m_settings;
        bool                    m_loaded;

    };

}

#endif

#endif