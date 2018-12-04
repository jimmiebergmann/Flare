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

#include "flare/window/private/win32Window.hpp"

#if defined(FLARE_PLATFORM_WINDOWS)

namespace Flare
{

    namespace Priv
    {

        Win32Window::Win32Window() :
            m_hWnd(nullptr),
            m_hInstance(nullptr),
            m_size(800, 600),
            m_title("Flare application"),
            m_open(false)
        {
            open(m_size, m_title);
        }

        Win32Window::Win32Window(const Vector2ui32 & size, const std::string & title) :
            Win32Window()
        {
            open(size, title);
        }

        Win32Window::~Win32Window()
        {
            close();
        };

        void Win32Window::open(const Vector2ui32 & size, const std::string & title)
        {

            m_open = true;
        }

        void Win32Window::close()
        {

        }

        void Win32Window::update()
        {

        }

        void Win32Window::show()
        {

        }

        void Win32Window::hide()
        {

        }

        bool Win32Window::isOpen()
        {
            return m_open;
        }

        HWND Win32Window::getHWnd() const
        {
            return m_hWnd;
        }

        HINSTANCE Win32Window::getHInstance() const
        {
            return m_hInstance;
        }

    }

}

#endif