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

#ifndef FLARE_SYSTEM_MEMORY_ALLOCATOR_HPP
#define FLARE_SYSTEM_MEMORY_ALLOCATOR_HPP

#include "flare/build.hpp"

namespace Flare
{

    template<typename Type, size_t N>
    class FLARE_API RamMemoryAllocator
    {

    public:

        template<const Type type>
        class Object
        {

        public:

            Type getType() const
            {
                return type;
            }

            int64_t getRamUsage() const
            {
                return m_ramUsage;
            }

            void setRamUsage(const size_t usage)
            {
                m_allocator.updateRamUsage<type>(m_ramUsage, usage);
                m_ramUsage = usage;

            }

            Object(RamMemoryAllocator<Type, N> & allocator) :
                m_allocator(allocator),
                m_ramUsage(0)
            { }

        private:

            Object(const Object &) = delete;

            RamMemoryAllocator<Type, N> & m_allocator;
            int64_t m_ramUsage;

        };

        RamMemoryAllocator() :
            m_ramUsage{0},
            m_totalRamUsage(0)

        { }

        template<Type type>
        void updateRamUsage(const int64_t prevUsage, const int64_t newUsage)
        {
            const int64_t diff = newUsage - prevUsage;
            m_totalRamUsage += diff;
            m_ramUsage[static_cast<size_t>(type)] += diff;
        }

    private:

        RamMemoryAllocator(const RamMemoryAllocator &) = delete;

        int64_t m_ramUsage[N];
        int64_t m_totalRamUsage;

    };

}

#include "memoryAllocator.inl"

#endif