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

        // Helper functions
        static std::wstring stringToWideString(const std::string & string)
        {
            int len;
            int slength = static_cast<int>(string.length() + 1);
            len = MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, 0, 0);
            wchar_t* buf = new wchar_t[len];
            MultiByteToWideChar(CP_ACP, 0, string.c_str(), slength, buf, len);
            std::wstring r(buf);
            delete[] buf;
            return r;
        }

        Win32Window::Win32Window() :
            m_open(false),
            m_visible(false),
            m_windowMode(WindowMode::Windowed),
            m_hWnd(nullptr),
            m_hInstance(nullptr),
            m_size(0, 0) 
        {
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
            if (m_open)
            {
                close();
            }

            // Get default styles.
            m_extendedStyle = WS_EX_APPWINDOW;
            m_style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_BORDER;

            m_title = title;
            m_size = size;

            // Generate class name.
            GUID guid = { 0 };
            const size_t guidArraySize = 64;
            wchar_t guidArray[guidArraySize] = { 0 };
            CoCreateGuid(&guid);
            StringFromGUID2(guid, guidArray, guidArraySize);
            std::wstring className = L"flare_" + std::wstring(guidArray);

            // Create a window class(WNDCLASS - win32)
            WNDCLASS winClass;
            HINSTANCE winInstance = GetModuleHandle(NULL); // Grab any old handle
            winClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
            winClass.lpfnWndProc = (WNDPROC)Win32Window::windowProcStatic;
            winClass.cbClsExtra = 0;
            winClass.cbWndExtra = 0;
            winClass.hInstance = winInstance;
            winClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
            winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
            winClass.hbrBackground = CreateSolidBrush(RGB(255, 255, 255));
            winClass.lpszClassName = className.c_str();
            winClass.lpszMenuName = NULL;

            // Register the window class
            if (!RegisterClass(&winClass))
            {
                throw std::runtime_error("Failed to register Window class.");
                return;
            }
            // The class is now registered.
            m_windowClassName = className;

            // Apply the style
            RECT windowRect;
            windowRect.left = static_cast<LONG>(0);
            windowRect.right = static_cast<LONG>(m_size.x);
            windowRect.top = static_cast<LONG>(0);
            windowRect.bottom = static_cast<LONG>(m_size.y);

            AdjustWindowRectEx(&windowRect, m_style, FALSE, m_extendedStyle);

            // Let's create the window.
            m_hWnd = CreateWindowEx(
                m_extendedStyle,
                className.c_str(),
                stringToWideString(m_title).c_str(),
                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | m_style,
                CW_USEDEFAULT,
                CW_USEDEFAULT,
                windowRect.right - windowRect.left,
                windowRect.bottom - windowRect.top,
                NULL,
                NULL,
                winInstance,
                this);

            // Did the window creation succeed?
            if (m_hWnd == NULL)
            {
                throw std::runtime_error("Failed to create window.");
            }

            // Get the device context
            m_hDc = GetDC(m_hWnd);

            m_open = true;
        }

        void Win32Window::close()
        {
            if (m_hDc && !ReleaseDC(m_hWnd, m_hDc))
            {
                throw std::runtime_error("Failed to release win32 device context.");
            }
            if (m_hWnd && !DestroyWindow(m_hWnd))
            {
                throw std::runtime_error("Failed to destroy win32 class.");
            }
            if (m_windowClassName.size() && !UnregisterClass(m_windowClassName.c_str(), m_hInstance))
            {
                throw std::runtime_error("Failed to unregister win32 class.");
            }
    
            m_hWnd = nullptr;
            m_hInstance = nullptr;
            m_hDc = nullptr;
            m_windowClassName.clear();
            m_open = false;
        }

        void Win32Window::update()
        {
            MSG message;
            while (PeekMessage(&message, NULL, NULL, NULL, PM_REMOVE))
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
        }

        void Win32Window::show()
        {
            ::ShowWindow(m_hWnd, TRUE);
            ::SetForegroundWindow(m_hWnd);
            ::SetFocus(m_hWnd);
            m_visible = true;
        }

        void Win32Window::hide()
        {
            ::ShowWindow(m_hWnd, FALSE);
            m_visible = false;
        }
        
        void Win32Window::setWindowMode(const WindowMode mode)
        {

        }

        void Win32Window::toggleWindowMode()
        {
            WindowMode newWindowMode = (m_windowMode == WindowMode::Windowed) ? WindowMode::Fullscreen : WindowMode::Windowed;

            m_windowMode = newWindowMode;
        }

        bool Win32Window::isOpen() const
        {
            return m_open;
        }

        bool Win32Window::isVisible() const
        {
            return m_visible;
        }

        bool Win32Window::isFullscreen() const
        {
            return m_windowMode == WindowMode::Fullscreen;
        }

        Win32Window::WindowMode Win32Window::getWindowMode() const
        {
            return m_windowMode;
        }

        const Vector2ui32 & Win32Window::getSize() const
        {
            return m_size;
        }

        const std::string & Win32Window::getTitle() const
        {
            return m_title;
        }

        HWND Win32Window::getHWnd() const
        {
            return m_hWnd;
        }

        HINSTANCE Win32Window::getHInstance() const
        {
            return m_hInstance;
        }

        LRESULT Win32Window::windowProcStatic(HWND p_HWND, UINT p_Message,
            WPARAM p_WParam, LPARAM p_LParam)
        {
            if (p_Message == WM_NCCREATE)
            {
                SetWindowLongPtr(p_HWND, GWLP_USERDATA, (LONG_PTR)((LPCREATESTRUCT)p_LParam)->lpCreateParams);
            }
            else
            {
                Win32Window * pWnd = (Win32Window*)GetWindowLongPtr(p_HWND, GWLP_USERDATA);

                if (pWnd != NULL)
                {
                    return pWnd->windowProc(p_HWND, p_Message, p_WParam, p_LParam);
                }
            }

            return DefWindowProc(p_HWND, p_Message, p_WParam, p_LParam);
        }

        LRESULT Win32Window::windowProc(HWND p_HWND, UINT p_Message,
            WPARAM p_WParam, LPARAM p_LParam)
        {
            switch (p_Message)
            {
                case WM_CLOSE:
                {
                    close();
                }
                break;
                default:
                    break;

            }

            return DefWindowProc(p_HWND, p_Message, p_WParam, p_LParam);
        }

    }

}

#endif