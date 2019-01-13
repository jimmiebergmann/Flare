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

#include <iostream>
#include "flare/flare.hpp"

int main(int argc, char ** argv)
{
    /*Flare::Window window({ 800, 600 }, "Flare - Default pipeline example.");

    Flare::RendererSettings settings;
    settings.setWindow(&window);

    std::unique_ptr<Flare::Renderer> renderer = std::make_unique<Flare::VulkanRenderer>(settings);
    
    //Flare::Scene scene;
    //scene.load("sponza.obj");


    window.show();
    while (window.update())
    {
        renderer->render();
    }*/

    

    Flare::Material mat;




    auto & out = mat.createOutputNode<Flare::Vector4f>(1.0f);
    out = mat.createVec4Node<float>(1.0f, 2.0f, 3.0f, 4.0f) * mat.createVec4Node<float>(5.0f, 6.0f, 7.0f, 8.0f);


    /*Flare::MaterialNode & node = out;
    mat.deleteNode(out);

    //mat.deleteNode(*out.getInput().getConnection()->getParent());

    auto & mult = mat.createMultVec4Vec4Node();
    */
    mat.debugPrint();

    /*mat.forEachNode([](Flare::MaterialNode & node)
    {
        std::cout << "Type: " << (int)node.getType() << std::endl;
    });*/

  

    //vec4->getInputX()->disconnect(a);

   // auto output = material.createOutputNode("output");

    /*output =*/ //Flare::Material::Vec3("name", 1.0f, 0.0f, 0.0f) * (Flare::Material::Texture2D("texture") = Flare::Material::Texture2DCoords("texture_coord"));

    /*output =*/ //(material.createTexture2DNode("texture") = material.createTexture2DCoordsNode("texture_coord")) * material.createVec4Node("name", 1.0f, 0.0f, 0.0f, 1.0f);

   
    return EXIT_SUCCESS;
}