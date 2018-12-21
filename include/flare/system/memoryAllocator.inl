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
    /*
    inline RenderMemoryAllocator::RenderMemoryAllocator() :
        m_totalUsage{ 0 ,0 },
        m_usage{ { 0 ,0 }, { 0 ,0 } }
    { }

    template<RenderMemoryObjectType type>
    inline RamMemoryObject<RenderMemoryObjectType> * RenderMemoryAllocator::createRamObject()
    {
        return new RamMemoryObject<RenderMemoryObjectType>(*this, type);
    }

    inline void RenderMemoryAllocator::setRamUsage(const RamMemoryObject<RenderMemoryObjectType> * memoryObject)
    {
        size_t * ptr = &m_totalUsage[static_cast<size_t>(memoryObject->getType())];
        int64_t used = *ptr + (memoryObject->getRamUsage() - memoryObject->getRamUsage());
        *ptr = static_cast<size_t>(used);
    }*/
    
  /*  template<enum class Type>
    template<>
    inline RamMemoryObject<Type> & RendererMemoryAllocator::createRamObject<RendererMemoryAllocator::Type::Pipeline>()
    {
        auto object = new MemoryObject<RendererMemoryAllocator>(*this);
        return *object;
    }*/

   /* template<RendererMemoryAllocator::Category category>
    inline size_t RendererMemoryAllocator::getTotalUsage() const
    {
         return m_totalUsage[static_cast<size_t>(category)];
    }

    template<RendererMemoryAllocator::Category category, RendererMemoryAllocator::Type type>
    inline void RendererMemoryAllocator::setUsage(const int64_t change) const
    {
       // m_usage[category][type] += change;
    }-*

   /* inline size_t RendererMemoryAllocator::getRamUsage() const
    {
        return m_totalRamUsage;
    }
    inline size_t RendererMemoryAllocator::getVramUsage() const
    {
        return m_totalVramUsage;
    }*/

   /* inline size_t RendererMemory::getTotaUsage() const
    {
        return m_memoryUsage;
    }

    inline void RendererMemory::incrementMemory(RendererObject<Texture> * child)
    {
        m_memoryUsage += child->getMemoryUsage();
    }

    inline void RendererMemory::incrementMemory(RendererObject<Pipeline> * child)
    {
        m_memoryUsage += child->getMemoryUsage();
    }

    inline void RendererMemory::decrementMemory(RendererObject<Texture> * child)
    {
        m_memoryUsage -= child->getMemoryUsage();
    }

    inline void RendererMemory::decrementMemory(RendererObject<Pipeline> * child)
    {
        m_memoryUsage -= child->getMemoryUsage();
    }*/

}
