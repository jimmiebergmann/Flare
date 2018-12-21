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

#include "flare/graphics/vulkan/vulkanCleaner.hpp"

#if defined(FLARE_VULKAN)

#include "flare/system/semaphore.hpp"
#include <chrono>

namespace Flare
{

    VulkanCleaner::VulkanCleaner() :
        m_running(false),
        m_refreshTime(0.0f),
        m_maxTime(0.0f)
    {
    }

    VulkanCleaner::~VulkanCleaner()
    {
        if (m_running)
        {
            m_running = false;
            m_thread.join();
        }

        std::lock_guard<std::mutex> lock(m_mutex);
        while (m_objects.size())
        {
            auto object = m_objects.front();
            m_objects.pop();
            delete object;
        }
    }

    void VulkanCleaner::start(float refreshTime, float maxTime)
    {
        if (m_running)
        {
            throw std::runtime_error("Cannot start already running cleaner.");
        }

        m_refreshTime = refreshTime;
        m_maxTime = maxTime;

        Semaphore sem;
        m_thread = std::thread([this, &sem]()
        {
            m_running = true;
            sem.notifyOne();

            while (m_running)
            {
                const auto startTime = std::chrono::high_resolution_clock::now();
                std::chrono::duration<double> elapsedTime(0.0f);

                while (1)
                {
                    std::lock_guard<std::mutex> lock(m_mutex);

                    if (m_objects.size() == 0)
                    {
                        elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
                        break;
                    }

                    auto object = m_objects.front();
                    m_objects.pop();

                    delete object;
                   
                    elapsedTime = std::chrono::high_resolution_clock::now() - startTime;
                    if (elapsedTime.count() >= m_maxTime)
                    {
                        break;
                    }
                }

                const auto sleepTime = std::chrono::milliseconds(static_cast<long long>(m_refreshTime * 1000.0f)) - elapsedTime;
                if (sleepTime.count() > 0)
                {
                    std::this_thread::sleep_for(sleepTime);
                }
            }

            
        });

        sem.wait();
    }

    void VulkanCleaner::stop()
    {
        if (!m_running)
        {
            return;
        }

        m_running = false;
        m_thread.join();
    }

    void VulkanCleaner::add(RenderObject * ptr)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_objects.push(ptr);
    }

}

#endif