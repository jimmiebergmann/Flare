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

#ifndef FLARE_PRIVATE_WINDOW_BASE_HPP
#define FLARE_PRIVATE_WINDOW_BASE_HPP

#include "flare/build.hpp"
#include "flare/math/vector.hpp"
#include <string>

namespace Flare
{

    namespace Priv
    {

        class FLARE_API WindowBase
        {

        public:

            enum class WindowMode
            {
                Windowed,
                Fullscreen
            };

            virtual ~WindowBase();

            virtual void open(const Vector2ui32 & size, const std::string & title) = 0;
            virtual void close() = 0;
            virtual bool update() = 0;
            virtual void show() = 0;
            virtual void hide() = 0;
            virtual void setWindowMode(const WindowMode mode) = 0;
            virtual void toggleWindowMode() = 0;
            virtual bool isOpen() const = 0;
            virtual bool isVisible() const = 0;
            virtual bool isFullscreen() const = 0;
            virtual WindowMode getWindowMode() const = 0;
            virtual const Vector2ui32 & getSize() const = 0;
            virtual const std::string & getTitle() const = 0;

        #if defined(FLARE_PLATFORM_WINDOWS)
            virtual HWND getHWnd() const = 0;
            virtual  HINSTANCE getHInstance() const = 0;
        #endif

        };


    }

}

#endif