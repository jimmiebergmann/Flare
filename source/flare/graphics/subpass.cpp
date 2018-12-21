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

#include "flare/graphics/subpass.hpp"

namespace Flare
{

    // Subpass.
    Subpass::~Subpass()
    { }


    // Multipass.
    std::vector<Subpass *>::const_iterator Multipass::addSubpass(Subpass * subpass)
    {
        auto it = std::find(m_subpasses.begin(), m_subpasses.end(), subpass);
        if (it != m_subpasses.end())
        {
            return it;
        }

        return m_subpasses.insert(m_subpasses.end(), subpass);
    }

    std::vector<Subpass *>::const_iterator Multipass::addSubpass(std::vector<Subpass *>::const_iterator position, Subpass * subpass)
    {
        auto it = std::find(m_subpasses.begin(), m_subpasses.end(), subpass);
        if (it != m_subpasses.end())
        {
            return it;
        }

        return m_subpasses.insert(position, subpass);
    }

    std::vector<Subpass *>::const_iterator Multipass::eraseSubpass(Subpass * subpass)
    {
        auto it = std::find(m_subpasses.begin(), m_subpasses.end(), subpass);
        if (it == m_subpasses.end())
        {
            return it;
        }

        return m_subpasses.erase(it);
    }

    std::vector<Subpass *>::const_iterator Multipass::firstSubpass() const
    {
        return m_subpasses.begin();
    }

    std::vector<Subpass *>::const_iterator Multipass::lastSubpass() const
    {
        return m_subpasses.end();
    }

}