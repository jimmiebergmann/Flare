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
#include "flare/window/window.hpp"
#include "flare/math/vector.hpp"
#include "flare/system/memoryAllocator.hpp"
#include <string>
#include <vector>
#include <optional>
#include <cstdlib>

namespace Flare
{

    class Texture;
    class Pipeline;

    class FLARE_API RendererSettings
    {

    public:

        RendererSettings(const int argc = 0, const char ** argv = nullptr);

        void setArguments(int argv, char ** argc);
        void setArguments(const std::vector<std::string> & argumnets);
        const std::vector<std::string> getArguments() const;

        void setMaxFrameRate(const float fps);
        void setUnlimitedFrameRate();
        float getMaxFrameRate() const;

        void setWindow(Window * window);
        Window * getWindow() const;

    #if defined(FLARE_PLATFORM_WINDOWS)
        void setHWnd(const HWND hWnd);
        HWND getHWnd() const;
        void setHInstance(const HINSTANCE hInstance);
        HINSTANCE getHInstance() const;
    #endif

    private:

        float m_frameRate;
        std::vector<std::string> m_arguments;

        // Window configurations.
        Window * m_pWindow;

    #if defined(FLARE_PLATFORM_WINDOWS)
        HWND m_hWnd;
        HINSTANCE m_hInstance;
    #endif


    };


    class FLARE_API RenderObject
    {

    public:

        virtual ~RenderObject();

    };

    enum class FLARE_API RenderObjectType
    {
        Texture,
        Framebuffer,
        Shader,
        ShaderProgram
    };

    typedef RamMemoryAllocator<RenderObjectType, 4> RenderMemoryAllocator;


    class FLARE_API Renderer
    {

    public:

        Renderer();
        virtual ~Renderer();

        virtual void setMaxFrameRate(const float fps) = 0;
        virtual void setUnlimitedFrameRate() = 0;
        virtual float getMaxFrameRate() const = 0;

        virtual void load(const RendererSettings & settings) = 0;
        virtual void update() = 0;
        virtual void render() = 0;
        virtual void resize(const Vector2ui32 & size) = 0;

        virtual const RenderMemoryAllocator & memory() const = 0;

        virtual std::shared_ptr<Texture> createTexture() = 0;
        virtual std::shared_ptr<Pipeline> createPipeline() = 0;

    private:

        Renderer(const Renderer &) = delete;

    };

}

#endif