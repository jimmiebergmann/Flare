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

#ifndef FLARE_GRAPHICS_SUBPASS_HPP
#define FLARE_GRAPHICS_SUBPASS_HPP

#include "flare/build.hpp"
#include <vector>

namespace Flare
{

    class Renderer;

    class FLARE_API Subpass
    {

    public:

        virtual ~Subpass();

        virtual void load(Renderer & renderer) = 0;

    };

    class Multipass : public Subpass
    {

    public:

        virtual void load(Renderer & renderer) = 0;

        std::vector<Subpass *>::const_iterator addSubpass(Subpass * subpass);
        std::vector<Subpass *>::const_iterator addSubpass(std::vector<Subpass *>::const_iterator position, Subpass * subpass);
        std::vector<Subpass *>::const_iterator eraseSubpass(Subpass * subpass);
        std::vector<Subpass *>::const_iterator firstSubpass() const;
        std::vector<Subpass *>::const_iterator lastSubpass() const;

    private:

        std::vector<Subpass *> m_subpasses;

    };

}

#endif