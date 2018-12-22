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

#ifndef FLARE_PRIVATE_WIN32_WINDOW_HPP
#define FLARE_PRIVATE_WIN32_WINDOW_HPP

#include "flare/build.hpp"

#if defined(FLARE_PLATFORM_WINDOWS)

#include "windowBase.hpp"

namespace Flare
{

    namespace Priv
    {

        class FLARE_API Win32Window : public WindowBase
        {

        public:

            Win32Window();
            Win32Window(const Vector2ui32 & size, const std::string & title);
            ~Win32Window();

            virtual void open(const Vector2ui32 & size, const std::string & title);
            virtual void close();
            virtual void update();
            virtual void show();
            virtual void hide();
            virtual void setWindowMode(const WindowMode mode);
            virtual void toggleWindowMode();
            virtual bool isOpen() const;
            virtual bool isVisible() const;
            virtual bool isFullscreen() const;
            virtual WindowMode getWindowMode() const;
            virtual const Vector2ui32 & getSize() const;
            virtual const std::string & getTitle() const;

            virtual HWND getHWnd() const;
            virtual HINSTANCE getHInstance() const;

        private:


            /**
            * Window event functions.
            *
            */
            static LRESULT windowProcStatic(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);
            LRESULT windowProc(HWND p_HWND, UINT p_Message, WPARAM p_WParam, LPARAM p_LParam);

            bool            m_open;
            bool            m_visible;
            WindowMode      m_windowMode;
            HWND            m_hWnd;
            HINSTANCE       m_hInstance;
            HDC			    m_hDc;
            Vector2ui32     m_size;
            std::string     m_title;
            DWORD           m_style;           ///< Win32 style of window.
            DWORD           m_extendedStyle;   ///< Win32 extended style of window.
            std::wstring    m_windowClassName;          

        };


    }

}

#endif

#endif