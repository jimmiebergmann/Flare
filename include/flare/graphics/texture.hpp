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

#ifndef FLARE_GRAPHICS_TEXTURE_HPP
#define FLARE_GRAPHICS_TEXTURE_HPP

#include "flare/build.hpp"
#include "flare/graphics/renderer.hpp"
#include "flare/math/vector.hpp"
#include "flare/system/memoryAllocator.hpp"

namespace Flare
{

    class FLARE_API Texture : public RenderMemoryAllocator::Object<RenderObjectType::Texture>, public RenderObject
    {

    public:

        enum class PixelFormat
        {
            RGB,
            RGBA
        }; 

        Texture(RenderMemoryAllocator & allocator);
        
        virtual ~Texture();

        virtual void load(const uint8_t * buffer = nullptr,
                          const Vector2ui32 & size = {0, 0},
                          const PixelFormat & pixelFormat = PixelFormat::RGBA,
                          const bool storeBuffer = false) = 0;
        
        virtual void load(const std::string & filename, const bool storeBuffer = false) = 0;

        virtual void unload() = 0;

        virtual const uint8_t * getBuffer() const = 0;

        virtual size_t getBufferSize() const = 0;

        virtual PixelFormat getPixelFormat() const = 0;

        virtual Vector2ui32 getSize() const = 0;

    private:

        Texture(const Texture &) = delete;
        

    };

}

#endif