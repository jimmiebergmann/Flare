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

#include "flare/graphics/material.hpp"
#include <iostream>

namespace Flare
{

    // Material node implementations.
    Material & MaterialNode::getMaterial() const
    {
        return m_material;
    }
    
    MaterialNode::MaterialNode(Material & material) :
        m_material(material)
    { }

    MaterialNode::~MaterialNode()
    { }


    // Material output node base implementations.
    MaterialOutputNodeBase::MaterialOutputNodeBase(Material & material) :
        MaterialNode(material)
    { }

    MaterialOutputNodeBase::~MaterialOutputNodeBase()
    { }


    // Material implementations.
    Material::Material()
    { }

    Material::~Material()
    {
        for (auto node : m_nodes)
        {
            delete node;
        }
    }

    void Material::deleteNode(MaterialNode & node)
    {
        auto pNode = &node;
        m_nodes.erase(pNode);
        if (node.getType() == MaterialNodeType::Output)
        {
            m_outputNodes.erase(static_cast<MaterialOutputNodeBase*>(pNode));
        }

        delete pNode;
    }

    void Material::forEachNode(std::function<void(MaterialNode &)> func)
    {
        for (auto it = m_nodes.begin(); it != m_nodes.end(); it++)
        {
            func(**it);
        }
    }

    void Material::forEachOutputNode(std::function<void(MaterialNode &)> func)
    {
        for (auto it = m_outputNodes.begin(); it != m_outputNodes.end(); it++)
        {
            func(**it);
        }
    }

    static void _debugPrintInput(MaterialNode * pNode, const uint32_t level)
    {
        switch (pNode->getType())
        {
        case MaterialNodeType::Output:
        {
            std::cout << std::string(level * 3, ' ') << "Output node: " << std::endl;

            MaterialOutputNode<Vector4f> * pCastedNode = static_cast<MaterialOutputNode<Vector4f>*>(pNode);
            auto connection = pCastedNode->getInput().getConnection();
            if (connection)
            {
                std::cout << std::string((level + 1) * 3, ' ') << "Input: ";
                _debugPrintInput(&connection->getNode(), level + 1);
            }
        }
        break;
        case MaterialNodeType::Vec4:
        {
            std::cout << std::string(level * 3, ' ') << "Vec4 node: (";

            MaterialVec4Node<float> * pCastedNode = static_cast<MaterialVec4Node<float>*>(pNode);
           
            std::cout << pCastedNode->getInputX().getValue() << ", ";
            std::cout << pCastedNode->getInputY().getValue() << ", ";
            std::cout << pCastedNode->getInputZ().getValue() << ", ";
            std::cout << pCastedNode->getInputW().getValue() << ")" << std::endl;
        }
        break;
        case MaterialNodeType::MultVec4Vec4:
        {
            std::cout << std::string(level * 3, ' ') << "MultVec4Vec4 node: " << std::endl;

            MaterialMultVec4Vec4Node<float> * pCastedNode = static_cast<MaterialMultVec4Vec4Node<float>*>(pNode);

            auto connectionA = pCastedNode->getInputA().getConnection();
            auto connectionB = pCastedNode->getInputB().getConnection();
            if (connectionA)
            {
                std::cout << std::string((level + 1) * 3, ' ') << "Input A: ";
                _debugPrintInput(&connectionA->getNode(), level + 1);
            }
            if (connectionB)
            {
                std::cout << std::string((level + 1) * 3, ' ') << "Input B: ";
                _debugPrintInput(&connectionB->getNode(), level + 1);
            }
        }
        break;
        default:
            break;
        }
    }

    void Material::debugPrint()
    {
        uint32_t i = 1; 
        for (auto it = m_outputNodes.begin(); it != m_outputNodes.end(); it++)
        {
            _debugPrintInput(*it, 0);
        }
    }

}