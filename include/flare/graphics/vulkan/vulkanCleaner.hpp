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

#ifndef FLARE_GRAPHICS_VULKAN_CLEANER_HPP
#define FLARE_GRAPHICS_VULKAN_CLEANER_HPP

#include "flare/build.hpp"

#if defined(FLARE_VULKAN)

#include "flare/graphics/renderer.hpp"
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>

namespace Flare
{

    class FLARE_API VulkanCleaner
    {

    public:

        VulkanCleaner();
        ~VulkanCleaner();

        void start(float refreshTime, float maxTime);
        void stop();
        void add(RenderObject * ptr);

    private:

        std::atomic<bool>           m_running;
        std::thread                 m_thread;
        std::mutex                  m_mutex;
        std::queue<RenderObject *>  m_objects;
        float                       m_refreshTime;
        float                       m_maxTime;

    };

}

#endif

#endif