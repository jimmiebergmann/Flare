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

#include "flare/graphics/renderer.hpp"

namespace Flare
{

    // Renderer settings.


    RendererSettings::RendererSettings(const int argc, const char ** argv) :
        m_debug(false),
        m_frameRate(0),
        m_pWindow(nullptr)
#if defined(FLARE_PLATFORM_WINDOWS)
        ,
        m_hWnd(nullptr),
        m_hInstance(nullptr)
#endif
    { }

    RendererSettings::RendererSettings(const RendererSettings & settings) :
        m_arguments(settings.m_arguments),
        m_debug(settings.m_debug),
        m_frameRate(settings.m_frameRate),
        m_pWindow(settings.m_pWindow)
#if defined(FLARE_PLATFORM_WINDOWS)
        ,
        m_hWnd(settings.m_hWnd),
        m_hInstance(settings.m_hInstance)
#endif
    { }

    void RendererSettings::setArguments(int argv, char ** argc)
    {
        // Parse arguments.
        // ...
        for (int i = 1; i < argv; i++)
        {
            m_arguments.push_back(argc[i]);
        }
    }

    void RendererSettings::setArguments(const std::vector<std::string> & argumnets)
    {
        m_arguments = argumnets;
    }

    const std::vector<std::string> RendererSettings::getArguments() const
    {
        return m_arguments;
    }

    void RendererSettings::setDebug(bool flag)
    {
        m_debug = flag;
    }

    bool RendererSettings::getDebug() const
    {
        return m_debug;
    }

    void RendererSettings::setMaxFrameRate(const float fps)
    {
        m_frameRate = fps;
    }

    void RendererSettings::setUnlimitedFrameRate()
    {
        m_frameRate = 0.0f;
    }

    float RendererSettings::getMaxFrameRate() const
    {
        return m_frameRate;
    }

    void RendererSettings::setWindow(Window * window)
    {
        m_pWindow = window;
    }

    Window * RendererSettings::getWindow() const
    {
        return m_pWindow;
    }

#if defined(FLARE_PLATFORM_WINDOWS)
  
    void RendererSettings::setHWnd(const HWND hWnd)
    {
        m_hWnd = hWnd;
    }

    HWND RendererSettings::getHWnd() const
    {
        return m_hWnd;
    }

    void RendererSettings::setHInstance(const HINSTANCE hInstance)
    {
        m_hInstance = hInstance;
    }

    HINSTANCE RendererSettings::getHInstance() const
    {
        return m_hInstance;
    }

#endif


    // Render object
    RenderObject::~RenderObject()
    {

    }


    // Renderer.
    Renderer::Renderer()
    { }

    Renderer::~Renderer()
    { }



}