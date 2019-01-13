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
#include <chrono>

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


    /*std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < 10000; i++)
    {*/
        auto & out = mat.createOutputNode<Flare::Vector4f>(1.0f);
        out = (mat.createVec4Node<float>(1.0f, 2.0f, 3.0f, 4.0f) * mat.createScalarNode<float>(10.0f)) * mat.createVec4Node<float>(5.0f, 6.0f, 7.0f, 8.0f);

        auto & out2 = mat.createOutputNode<Flare::Vector4i32>(1.0f) = mat.createVec4Node<int32_t>(11, 22, 33, 44);

   /* }

    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << "It took me " << time_span.count() << " seconds.";
    */
    //mat.debugPrint();
    std::cout << std::endl << "-------------------------------------------" << std::endl << std::endl;
    
    std::string glslSource = "";
    mat.generateGlsl(glslSource);
    std::cout << glslSource << std::endl;
    
  

    //vec4->getInputX()->disconnect(a);

   // auto output = material.createOutputNode("output");

    /*output =*/ //Flare::Material::Vec3("name", 1.0f, 0.0f, 0.0f) * (Flare::Material::Texture2D("texture") = Flare::Material::Texture2DCoords("texture_coord"));

    /*output =*/ //(material.createTexture2DNode("texture") = material.createTexture2DCoordsNode("texture_coord")) * material.createVec4Node("name", 1.0f, 0.0f, 0.0f, 1.0f);

   
    return EXIT_SUCCESS;
}