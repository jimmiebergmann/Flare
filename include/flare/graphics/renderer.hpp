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
#include "flare/window/windowProxy.hpp"
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
        RendererSettings(const RendererSettings & settings);

        void setArguments(const int argc, const char ** argv);
        void setArguments(const std::vector<std::string> & argumnets);
        const std::vector<std::string> getArguments() const;

        void setDebug(bool flag);
        bool getDebug() const;

        void setMaxFrameRate(const float fps);
        void setUnlimitedFrameRate();
        float getMaxFrameRate() const;

        void setWindow(Window * window);
        Window * getWindow() const;

        void setWindowProxy(const WindowProxy & windowProxy);
        const WindowProxy & getWindowProxy() const;

    private:

        std::vector<std::string> m_arguments;
        bool m_debug;
        float m_frameRate;

        // Window configurations.
        Window * m_pWindow;
        WindowProxy m_windowProxy;

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
        ShaderProgram,
        VertexArray,
        VertexBuffer,
    };

    typedef RamMemoryAllocator<RenderObjectType, 4> RenderMemoryAllocator;


    class FLARE_API Renderer
    {

    public:

        Renderer();
        virtual ~Renderer();

        virtual void load(const RendererSettings & settings) = 0;
        virtual void unload() = 0;
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