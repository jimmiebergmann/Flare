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

#ifndef FLARE_GRAPHICS_RENDERER_HPP
#define FLARE_GRAPHICS_RENDERER_HPP

#include "flare/build.hpp"

namespace Flare
{

    class Texture;

    class FLARE_API Renderer
    {

    public:

        Renderer();
        virtual ~Renderer();

    #if defined(FLARE_PLATFORM_WINDOWS)
        virtual void setHwnd(const HWND hwnd) = 0;
        virtual HWND getHwnd() const = 0;
        virtual void setHinstance(const HINSTANCE hinstance) = 0;
        virtual HINSTANCE getHinstance() const = 0;
    #endif

        virtual void setMaxFrameRate(const float fps) = 0;
        virtual void setUnlimitedFrameRate() = 0;
        virtual float getMaxFrameRate() const = 0;

        virtual void load() = 0;
        virtual void update() = 0;
        virtual void render() = 0;

        virtual Texture * createTexture() = 0;

    };

}

#endif