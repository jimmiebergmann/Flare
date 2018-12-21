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

#ifndef FLARE_GRAPHICS_VULKAN_TEXTURE_HPP
#define FLARE_GRAPHICS_VULKAN_TEXTURE_HPP

#include "flare/graphics/texture.hpp"

#if defined(FLARE_VULKAN)

namespace Flare
{

    class VulkanRenderer;

    class FLARE_API VulkanTexture : public Texture
    {

    public:

        ~VulkanTexture();

        virtual void load(const uint8_t * buffer = nullptr,
                          const Vector2ui32 & size = { 0, 0 },
                          const PixelFormat & pixelFormat = PixelFormat::RGBA,
                          const bool storeBuffer = false);

        virtual void unload();

        virtual Vector2ui32 getSize() const;

    private:

        VulkanTexture(RenderMemoryAllocator & allocator);
        VulkanTexture(const VulkanTexture &) = delete;

        Vector2ui32 m_size;

        friend class VulkanRenderer;

    };

}

#endif

#endif