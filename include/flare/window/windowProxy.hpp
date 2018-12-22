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

#ifndef FLARE_WINDOW_PROXY_HPP
#define FLARE_WINDOW_PROXY_HPP

#include "flare/build.hpp"
#include "flare/window/window.hpp"
#include <optional>

namespace Flare
{

    class FLARE_API WindowProxy
    {

    public:

        WindowProxy();
        WindowProxy(const Window & window);

    #if defined(FLARE_PLATFORM_WINDOWS)

    public:

        WindowProxy(const HWND hWnd, const HINSTANCE hInstance);

        HWND getHWnd() const;
        void setHWnd(const HWND hWnd);
        HINSTANCE getHInstance() const;
        void setHInstance(const HINSTANCE hInstance);

    private:

        HWND m_hWnd;
        HINSTANCE m_hInstance;

    #endif    

    };

}

#include "windowProxy.inl"

#endif