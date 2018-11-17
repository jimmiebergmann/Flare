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

namespace Flare
{

    class FLARE_API VulkanRenderer : public Renderer
    {

    public:

        VulkanRenderer();
        ~VulkanRenderer();

#if defined(FLARE_PLATFORM_WINDOWS)
        virtual void setHwnd(const HWND hwnd);
        virtual HWND getHwnd() const;
        virtual void setHinstance(const HINSTANCE hinstance);
        virtual HINSTANCE getHinstance() const;
#endif

        virtual void setMaxFrameRate(const float fps);
        virtual void setUnlimitedFrameRate();
        virtual float getMaxFrameRate() const;

        virtual void load();
        virtual void update();
        virtual void render();

        virtual Texture * createTexture();

    private:

        VulkanRenderer(const VulkanRenderer &) = delete;

    };

}

#endif

#endif