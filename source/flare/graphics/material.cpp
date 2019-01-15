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

    // Material pin base implementations.
    MaterialPinBase::~MaterialPinBase()
    { }


    // Material pin implementations.
    MaterialPin::~MaterialPin()
    { }

    MaterialNode & MaterialPin::getNode()
    {
        return m_node;
    }

    const MaterialNode & MaterialPin::getNode() const
    {
        return m_node;
    }

    const std::string & MaterialPin::getName() const
    {
        return m_name;
    }

    MaterialPin::MaterialPin(MaterialNode & node, const std::string & name) :
        m_node(node),
        m_name(name)
    { }


    // Material input pin base implementations.
    MaterialInputPinBase::~MaterialInputPinBase()
    { }

    MaterialInputPinBase::MaterialInputPinBase(MaterialNode & node, const std::string & name) :
        MaterialPin(node, name)
    { }


    // Material out pin base implementations.
    MaterialOutputPinBase::~MaterialOutputPinBase()
    { }

    MaterialOutputPinBase::MaterialOutputPinBase(MaterialNode & node, const std::string & name) :
        MaterialPin(node, name)
    { }


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

    // Material vec4 node base implementations.
    MaterialVec4NodeBase::~MaterialVec4NodeBase()
    { }

    MaterialVec4NodeBase::MaterialVec4NodeBase(Material & material) :
        MaterialNode(material)
    { }


    // Material mult vec4-vec4 base implementations.
    MaterialMultVec4Vec4NodeBase::~MaterialMultVec4Vec4NodeBase()
    { }

    MaterialMultVec4Vec4NodeBase::MaterialMultVec4Vec4NodeBase(Material & material) :
        MaterialNode(material)
    {

    }

    // Material output node base implementations.
    MaterialOutputNodeBase::MaterialOutputNodeBase(Material & material) :
        MaterialNode(material)
    { }

    MaterialOutputNodeBase::~MaterialOutputNodeBase()
    { }


    // Material GLSL generator implementations.
    void MaterialGlslGenerator::run(const Material & material, std::string & source)
    {
        auto & outputNodes = material.getOutputNodes();
        size_t outputNodeIndex = 1;
        for (auto it = outputNodes.begin(); it != outputNodes.end(); it++)
        {
            const auto node = *it;
            m_outputNodes.insert({ "out_" + std::to_string(outputNodeIndex), node });
            outputNodeIndex++;
        }

        traverseOutputNodes(outputNodes);
        

        for (auto outputNode : m_outputNodes)
        {
            source += "out " + getVarAsString(outputNode.second->getDataType()) + " " + outputNode.first + ";\n";
             
        }
        
        source += "void main(void){\n";

        source += "}\n";

    }

    void MaterialGlslGenerator::traverseOutputNodes(const std::list<MaterialOutputNodeBase *> & outputNodes)
    {
        for (const auto outputNode : outputNodes)
        {
            traverseNode(outputNode);
        }

    }

    void MaterialGlslGenerator::traverseNode(const MaterialNode * node)
    {
        switch (node->getType())
        {
            case MaterialNodeType::Scalar:          traverseDeclaration<MaterialNodeType::Scalar>(node);        break;
            case MaterialNodeType::Vec4:            traverseDeclaration<MaterialNodeType::Vec4>(node);          break;
            case MaterialNodeType::Output:          traverseOutput<MaterialNodeType::Output>(node);             break;
            case MaterialNodeType::MultVec4Vec4:    traverseOperator<MaterialNodeType::MultVec4Vec4>(node);     break;
            case MaterialNodeType::MultVec4Scalar:  traverseOperator<MaterialNodeType::MultVec4Scalar>(node);   break;
            default:                                throw std::runtime_error("Unknown material node type.");    break;
        }
    }


    std::string MaterialGlslGenerator::getVarAsString(MaterialDataType dataType)
    {
        switch (dataType)
        {
            case MaterialDataType::Boolean:     return "bool";  break;
            case MaterialDataType::Integer:     return "int";   break;
            case MaterialDataType::Float:       return "float"; break;
            case MaterialDataType::Vec2Boolean: return "bvec2"; break;
            case MaterialDataType::Vec2Integer: return "ivec2"; break;
            case MaterialDataType::Vec2Float:   return "vec2";  break;
            case MaterialDataType::Vec3Boolean: return "bvec3"; break;
            case MaterialDataType::Vec3Integer: return "ivec3"; break;
            case MaterialDataType::Vec3Float:   return "vec3";  break;
            case MaterialDataType::Vec4Boolean: return "bvec4"; break;
            case MaterialDataType::Vec4Integer: return "ivec4"; break;
            case MaterialDataType::Vec4Float:   return "vec4";  break;
            default: break;
        }
        throw std::runtime_error("Could not getVarAsString");
        return "";
    }


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
            auto outputNode = static_cast<MaterialOutputNodeBase*>(pNode);
            auto it = std::find(m_outputNodes.begin(), m_outputNodes.end(), outputNode);
            if (it != m_outputNodes.end())
            {
                m_outputNodes.erase(it);
            }
        }

        delete pNode;
    }

    const std::set<MaterialNode *> & Material::getNodes() const
    {
        return m_nodes;
    }

    const std::list<MaterialOutputNodeBase *> & Material::getOutputNodes() const
    {
        return m_outputNodes;
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

            MaterialOutputNodeBase * pNodeBase = static_cast<MaterialOutputNodeBase*>(pNode);

            MaterialOutputNode<Vector4f> * pCastedNode = static_cast<MaterialOutputNode<Vector4f>*>(pNode);
            auto connection = pCastedNode->getInput().getConnection();
            if (connection)
            {
                std::cout << std::string((level + 1) * 3, ' ') << "Input: ";
                _debugPrintInput(&connection->getNode(), level + 1);
            }
        }
        break;
        case MaterialNodeType::Scalar:
        {
           /* std::cout << std::string(level * 3, ' ') << "Vec4 node: (";

            MaterialScalarNode<float> * pCastedNode = static_cast<MaterialScalarNode<float>*>(pNode);

            std::cout << pCastedNode->getInput().getValue() << ")" << std::endl;*/
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
        case MaterialNodeType::MultVec4Scalar:
        {
            /*std::cout << std::string(level * 3, ' ') << "MultVec4Scalar node: " << std::endl;

            MaterialMultVec4ScalarNode<float> * pCastedNode = static_cast<MaterialMultVec4ScalarNode<float>*>(pNode);

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
            }*/
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

    void Material::generateGlsl(std::string & source)
    {
        MaterialGlslGenerator generator;
        generator.run(*this, source);
    }

}