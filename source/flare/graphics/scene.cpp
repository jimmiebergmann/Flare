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

#include "flare/graphics/scene.hpp"
#include "tinyobjloader/tiny_obj_loader.h"
#include <algorithm>

namespace Flare
{

    static void GetFileDirectory(const std::string & path, std::string & directory);

    Scene::Scene()
    { }

    Scene::~Scene()
    { }

    void Scene::load(const std::string & filename)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;

        std::string directory;
        GetFileDirectory(filename, directory);

        std::string warn;
        std::string err;
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
            filename.c_str(), directory.c_str(), true);

        int foo = 5;
    }

    void GetFileDirectory(const std::string & path, std::string & directory)
    {
        auto pos = path.find_last_of("/\\");
        if (pos == 0 || pos == std::string::npos)
        {
            directory = "";
        }
        else
        {
            directory = path.substr(0, pos);
        }
    }


}