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

namespace Flare
{

    VulkanRenderer::VulkanRenderer()
    {
    }

    VulkanRenderer::~VulkanRenderer()
    {
    }

#if defined(FLARE_PLATFORM_WINDOWS)

    void VulkanRenderer::setHwnd(const HWND hwnd)
    {
    }

    HWND VulkanRenderer::getHwnd() const
    {
        return 0;
    }

    void VulkanRenderer::setHinstance(const HINSTANCE hinstance)
    {
    }

    HINSTANCE VulkanRenderer::getHinstance() const
    {
        return 0;
    }

#endif

    void VulkanRenderer::setMaxFrameRate(const float fps)
    {
    }

    void VulkanRenderer::setUnlimitedFrameRate()
    {
    }
    
    float VulkanRenderer::getMaxFrameRate() const
    {
        return 0.0f;
    }

    void VulkanRenderer::load()
    {
    }

    void VulkanRenderer::update()
    {
    }

    void VulkanRenderer::render()
    {
    }

    Texture * VulkanRenderer::createTexture()
    {
        Texture * texture = new VulkanTexture();
        return texture;
    }

}

#endif