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

namespace Flare
{

#if defined(FLARE_PLATFORM_WINDOWS)
        
    inline WindowProxy::WindowProxy() :
        m_hWnd(0),
        m_hInstance(0)
    { }

    inline WindowProxy::WindowProxy(const Window & window) :
        m_hWnd(window.getHWnd()),
        m_hInstance(window.getHInstance())
    { }

    inline WindowProxy::WindowProxy(const HWND hWnd, const HINSTANCE hInstance) :
        m_hWnd(hWnd),
        m_hInstance(hInstance)
    { }

    inline HWND WindowProxy::getHWnd() const
    {
        return m_hWnd;
    }

    inline void WindowProxy::setHWnd(const HWND hWnd)
    {
        m_hWnd = hWnd;
    }

    inline HINSTANCE WindowProxy::getHInstance() const
    {
        return m_hInstance;
    }

    inline void WindowProxy::setHInstance(const HINSTANCE hInstance)
    {
        m_hInstance = hInstance;
    }

#endif    

}