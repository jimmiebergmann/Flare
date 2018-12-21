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

#define CHECK_LOADED \
    if(!m_loaded) { throw std::runtime_error("Renderer has not been loaded."); }\

#define LAMBA_ADD_TO_CLEANER \
    [this](auto ptr) { m_cleaner.add(ptr); }


namespace Flare
{

    VulkanRenderer::VulkanRenderer() :
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
        m_cleaner.stop();
    }

    void VulkanRenderer::setMaxFrameRate(const float fps)
    {
        m_maxFrameRate = fps;
    }

    void VulkanRenderer::setUnlimitedFrameRate()
    {
        m_maxFrameRate = 0.0f;
    }

    float VulkanRenderer::getMaxFrameRate() const
    {
        return m_maxFrameRate;
    }

    void VulkanRenderer::load(const RendererSettings & settings)
    {

        #if defined(FLARE_PLATFORM_WINDOWS)
            Window * pWindow = settings.getWindow();
            HWND hWnd = settings.getHWnd();
            if (hWnd == nullptr)
            {
                if (pWindow)
                {
                    hWnd = pWindow->getHWnd();
                }
            }

            HINSTANCE hInstance = settings.getHInstance();
            if (hInstance == nullptr)
            {
                if (pWindow)
                {
                    hInstance = pWindow->getHInstance();
                }
            }

            if (hWnd && hInstance)
            {

            }
        #endif


       m_cleaner.start(0.01f, 0.01f);
       m_loaded = true;
    }

    void VulkanRenderer::update()
    {
    }

    void VulkanRenderer::render()
    {
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
        //auto ptr = std::make_shared<Pipeline>();
        //m_memory->incrementMemory(texture);
        return ptr;
    }

}

#endif