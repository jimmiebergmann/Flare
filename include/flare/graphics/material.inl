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

    // Get data type implementations.
    template <>
    inline MaterialDataType GetDataType<bool>::value()
    {
        return MaterialDataType::Boolean;
    }
    template <>
    inline MaterialDataType GetDataType<int32_t>::value()
    {
        return MaterialDataType::Integer;
    }
    template <>
    inline MaterialDataType GetDataType<float>::value()
    {
        return MaterialDataType::Float;
    }
    template <>
    inline MaterialDataType GetDataType<Vector2b>::value()
    {
        return MaterialDataType::Vec2Boolean;
    }
    template <>
    inline MaterialDataType GetDataType<Vector2i32>::value()
    {
        return MaterialDataType::Vec2Integer;
    }
    template <>
    inline MaterialDataType GetDataType<Vector2f>::value()
    {
        return MaterialDataType::Vec2Float;
    }
    template <>
    inline MaterialDataType GetDataType<Vector3b>::value()
    {
        return MaterialDataType::Vec3Boolean;
    }
    template <>
    inline MaterialDataType GetDataType<Vector3i32>::value()
    {
        return MaterialDataType::Vec3Integer;
    }
    template <>
    inline MaterialDataType GetDataType<Vector3f>::value()
    {
        return MaterialDataType::Vec3Float;
    }
    template <>
    inline MaterialDataType GetDataType<Vector4b>::value()
    {
        return MaterialDataType::Vec4Boolean;
    }
    template <>
    inline MaterialDataType GetDataType<Vector4i32>::value()
    {
        return MaterialDataType::Vec4Integer;
    }
    template <>
    inline MaterialDataType GetDataType<Vector4f>::value()
    {
        return MaterialDataType::Vec4Float;
    }


    // Material input pin implementations.
    template<typename T>
    inline MaterialDataType MaterialInputPin<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template<>
    inline MaterialInputPin<bool>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, false)
    { }
    template<>
    inline MaterialInputPin<int32_t>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, 0)
    { }
    template<>
    inline MaterialInputPin<float>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, 0.0f)
    { }

    template<>
    inline MaterialInputPin<Vector2b>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector2b(false))
    { }
    template<>
    inline MaterialInputPin<Vector2i32>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector2i32(0))
    { }
    template<>
    inline MaterialInputPin<Vector2f>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector2f(0.0f))
    { }

    template<>
    inline MaterialInputPin<Vector3b>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector3b(false))
    { }
    template<>
    inline MaterialInputPin<Vector3i32>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector3i32(0))
    { }
    template<>
    inline MaterialInputPin<Vector3f>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector3f(0.0f))
    { }

    template<>
    inline MaterialInputPin<Vector4b>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector4b(false))
    { }
    template<>
    inline MaterialInputPin<Vector4i32>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector4i32(0))
    { }
    template<>
    inline MaterialInputPin<Vector4f>::MaterialInputPin(MaterialNode & node, const std::string & name) :
        MaterialInputPin(node, name, Vector4f(0.0f))
    { }

    template<typename T>
    template<typename ... Args>
    inline MaterialInputPin<T>::MaterialInputPin(MaterialNode & node, const std::string & name, Args ... value) :
        MaterialInputPinBase(node, name),
        m_value(value...),
        m_connection(nullptr)
    { }

    template<typename T>
    inline MaterialInputPin<T>::~MaterialInputPin()
    {
        disconnect();
    }

    template<typename T>
    inline const T & MaterialInputPin<T>::getValue() const
    {
        return m_value;
    }

    template<typename T>
    inline void MaterialInputPin<T>::setValue(const T & value)
    {
        m_value = value;
    }

    template<typename T>
    MaterialOutputPin<T> * MaterialInputPin<T>::getConnection()
    {
        return m_connection;
    }
    template<typename T>
    const MaterialOutputPin<T> * MaterialInputPin<T>::getConnection() const
    {
        return m_connection;
    }

    template<typename T>
    inline void MaterialInputPin<T>::connect(MaterialOutputPin<T> & output)
    {
        disconnect();
        m_connection = &output;

        output.m_connections.insert(this);
    }

    template<typename T>
    inline void MaterialInputPin<T>::disconnect()
    {
        if (m_connection == nullptr)
        {
            return;
        }

        m_connection->m_connections.erase(this);
        m_connection = nullptr;
    }

    template<typename T>
    MaterialInputPin<T> & MaterialInputPin<T>::operator = (MaterialSingleOutputPin<T> & node)
    {
        connect(node.getOutput());
        return *this;
    }

    template<typename T>
    MaterialInputPin<T> & MaterialInputPin<T>::operator = (MaterialOutputPin<T> & outputPin)
    {
        connect(outputPin);
        return *this;
    }

    template<typename T>
    inline MaterialOutputPinBase * MaterialInputPin<T>::getConnectionBase()
    {
        return m_connection;
    }
    template<typename T>
    inline const MaterialOutputPinBase * MaterialInputPin<T>::getConnectionBase() const
    {
        return m_connection;
    }


    // Material output pin implementations.
    template<typename T>
    inline MaterialDataType MaterialOutputPin<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template<typename T>
    inline MaterialOutputPin<T>::MaterialOutputPin(MaterialNode & node, const std::string & name) :
        MaterialOutputPinBase(node, name)
    { }

    template<typename T>
    inline MaterialOutputPin<T>::~MaterialOutputPin()
    {
        disconnectAll();
    }

    template<typename T>
    inline void MaterialOutputPin<T>::connect(MaterialInputPin<T> & input)
    {
        input.connect(this);
    }

    template<typename T>
    inline void MaterialOutputPin<T>::disconnect(MaterialInputPin<T> & input)
    {
        auto it = m_connections.find(&input);
        if (it == m_connections.end())
        {
            return;
        }

        auto connection = *it;
        m_connections.erase(connection);
        connection->m_connection = nullptr;
    }

    template<typename T>
    inline void MaterialOutputPin<T>::disconnectAll()
    {
        for (auto connection : m_connections)
        {
            connection->m_connection = nullptr;
        }
        m_connections.clear();
    }

    
    // Material single output node implementations.
    template <typename T>
    inline MaterialOutputPin<T> & MaterialSingleOutputPin<T>::getOutput()
    {
        return m_output;
    }
    template <typename T>
    inline const MaterialOutputPin<T> & MaterialSingleOutputPin<T>::getOutput() const
    {
        return m_output;
    }

    template <typename T>
    inline MaterialSingleOutputPin<T>::MaterialSingleOutputPin(MaterialNode * node) :
        MaterialSingleOutputPin(node, "out")
    { }

    template <typename T>
    inline MaterialSingleOutputPin<T>::MaterialSingleOutputPin(MaterialNode * node, const std::string & pinName) :
        m_output(*node, pinName)
    { }

    
    // Material scalar node implementations.
    /*template <typename T>
    MaterialNodeType MaterialScalarNode<T>::getType() const
    {
        return MaterialNodeType::Scalar;
    }

    template <typename T>
    MaterialInputPin<T> & MaterialScalarNode<T>::getInput()
    {
        return m_input;
    }

    template <typename T>
    MaterialScalarNode<T>::MaterialScalarNode(Material & material) :
        MaterialSingleOutputPin<T>(material, *this),
        m_input(*this, "value")
    { }

    template <typename T>
    MaterialScalarNode<T>::MaterialScalarNode(Material & material, const T scalar) :
        MaterialSingleOutputPin<T>(material, *this),
        m_input(*this, "value", scalar)
    { }

    template <typename T>
    MaterialScalarNode<T>::~MaterialScalarNode()
    {
        m_input.disconnect();
    }*/


    // Material Vec4 node implementations.
    template <typename T>
    MaterialDataType MaterialVec4Node<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template <typename T>
    inline MaterialNodeType MaterialVec4Node<T>::getType() const
    {
        return MaterialNodeType::Vec4;
    }

    template <typename T>
    inline MaterialInputPin<T> & MaterialVec4Node<T>::getInputX()
    {
        return m_inputX;
    }

    template <typename T>
    inline MaterialInputPin<T> & MaterialVec4Node<T>::getInputY()
    {
        return m_inputY;
    }

    template <typename T>
    inline MaterialInputPin<T> & MaterialVec4Node<T>::getInputZ()
    {
        return m_inputZ;
    }

    template <typename T>
    inline MaterialInputPin<T> & MaterialVec4Node<T>::getInputW()
    {

        return m_inputW;
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialVec4Node<T>::operator *(MaterialOutputPin<T> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialVec4Node<T>::operator *(MaterialSingleOutputPin<T> & node)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialVec4Node<T>::operator *(MaterialOutputPin<Vector4<T>> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialVec4Node<T>::operator *(MaterialSingleOutputPin<Vector4<T>> & node)
    {
        Material & mat = MaterialNode::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    inline MaterialVec4Node<T>::MaterialVec4Node(Material & material) :
        MaterialVec4NodeBase(material),
        MaterialSingleOutputPin<Vector4<T>>(this),
        m_inputX(*this, "x"),
        m_inputY(*this, "y"),
        m_inputZ(*this, "z"),
        m_inputW(*this, "w")
    { }

    template <typename T>
    inline MaterialVec4Node<T>::MaterialVec4Node(Material & material, const T scalar) :
        MaterialVec4NodeBase(material),
        MaterialSingleOutputPin<Vector4<T>>(this),
        m_inputX(*this, "x", scalar),
        m_inputY(*this, "y", scalar),
        m_inputZ(*this, "z", scalar),
        m_inputW(*this, "w", scalar)
    { }

    template <typename T>
    inline MaterialVec4Node<T>::MaterialVec4Node(Material & material, const T x, const T y, const T z, const T w) :
        MaterialVec4NodeBase(material),
        MaterialSingleOutputPin<Vector4<T>>(this),
        m_inputX(*this, "x", x),
        m_inputY(*this, "y", y),
        m_inputZ(*this, "z", z),
        m_inputW(*this, "w", w)
    { }

    template <typename T>
    inline MaterialVec4Node<T>::~MaterialVec4Node()
    {
        m_inputX.disconnect();
        m_inputY.disconnect();
        m_inputZ.disconnect();
        m_inputW.disconnect();
    }

    template <typename T>
    MaterialInputPinBase * MaterialVec4Node<T>::getInputXBase()
    {
        return &m_inputX;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialVec4Node<T>::getInputXBase() const
    {
        return &m_inputX;
    }
   
    template <typename T>
    MaterialInputPinBase * MaterialVec4Node<T>::getInputYBase()
    {
        return &m_inputY;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialVec4Node<T>::getInputYBase() const
    {
        return &m_inputY;
    }
    
    template <typename T>
    MaterialInputPinBase * MaterialVec4Node<T>::getInputZBase()
    {
        return &m_inputZ;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialVec4Node<T>::getInputZBase() const
    {
        return &m_inputZ;
    }
    
    template <typename T>
    MaterialInputPinBase * MaterialVec4Node<T>::getInputWBase()
    {
        return &m_inputW;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialVec4Node<T>::getInputWBase() const
    {
        return &m_inputW;
    }
    
    template <typename T>
    MaterialOutputPinBase * MaterialVec4Node<T>::getOutputBase()
    {
        return &(MaterialSingleOutputPin<Vector4<T>>::m_output);
    }
    template <typename T>
    const MaterialOutputPinBase * MaterialVec4Node<T>::getOutputBase() const
    {
        return &(MaterialSingleOutputPin<Vector4<T>>::m_output);
    }


    // Material mult vec4-vec4 implementations.
    template <typename T>
    inline MaterialInputPin<Vector4<T>> & MaterialMultVec4Vec4Node<T>::getInputA()
    {
        return m_inputA;
    }

    template <typename T>
    inline MaterialInputPin<Vector4<T>> & MaterialMultVec4Vec4Node<T>::getInputB()
    {
        return m_inputB;
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialMultVec4Vec4Node<T>::operator *(MaterialOutputPin<T> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialMultVec4Vec4Node<T>::operator *(MaterialSingleOutputPin<T> & node)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialMultVec4Vec4Node<T>::operator *(MaterialOutputPin<Vector4<T>> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialMultVec4Vec4Node<T>::operator *(MaterialSingleOutputPin<Vector4<T>> & node)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    inline MaterialNodeType MaterialMultVec4Vec4Node<T>::getType() const
    {
        return MaterialNodeType::MultVec4Vec4;
    }

    template <typename T>
    MaterialDataType MaterialMultVec4Vec4Node<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template <typename T>
    inline MaterialMultVec4Vec4Node<T>::MaterialMultVec4Vec4Node(Material & material) :
        MaterialMultVec4Vec4NodeBase(material),
        MaterialSingleOutputPin<Vector4<T>>(this),
        m_inputA(*this, "a"),
        m_inputB(*this, "b")
    { }

    template <typename T>
    inline MaterialMultVec4Vec4Node<T>::MaterialMultVec4Vec4Node(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<Vector4<T>> & pinB) :
        MaterialMultVec4Vec4Node(material)
    {
        m_inputA.connect(pinA);
        m_inputB.connect(pinB);
    }

    template <typename T>
    inline MaterialMultVec4Vec4Node<T>::~MaterialMultVec4Vec4Node()
    {
        m_inputA.disconnect();
        m_inputB.disconnect();
    }

    template <typename T>
    MaterialInputPinBase * MaterialMultVec4Vec4Node<T>::getInputABase()
    {
        return &m_inputA;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialMultVec4Vec4Node<T>::getInputABase() const
    {
        return &m_inputA;
    }

    template <typename T>
    MaterialInputPinBase * MaterialMultVec4Vec4Node<T>::getInputBBase()
    {
        return &m_inputB;
    }
    template <typename T>
    const MaterialInputPinBase * MaterialMultVec4Vec4Node<T>::getInputBBase() const
    {
        return &m_inputB;
    }

    template <typename T>
    MaterialOutputPinBase * MaterialMultVec4Vec4Node<T>::getOutputBase()
    {
        return &(MaterialSingleOutputPin<Vector4<T>>::m_output);
    }
    template <typename T>
    const MaterialOutputPinBase * MaterialMultVec4Vec4Node<T>::getOutputBase() const
    {
        return &(MaterialSingleOutputPin<Vector4<T>>::m_output);
    }


    // Material mult vec4-scalar implementations.
    /*template <typename T>
    MaterialDataType MaterialMultVec4ScalarNode<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template <typename T>
    MaterialNodeType MaterialMultVec4ScalarNode<T>::getType() const
    {
        return MaterialNodeType::MultVec4Scalar;
    }

    template <typename T>
    MaterialInputPin<Vector4<T>> & MaterialMultVec4ScalarNode<T>::getInputA()
    {
        return m_inputA;
    }

    template <typename T>
    MaterialInputPin<T> & MaterialMultVec4ScalarNode<T>::getInputB()
    {
        return m_inputB;
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialMultVec4ScalarNode<T>::operator *(MaterialOutputPin<T> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T> & MaterialMultVec4ScalarNode<T>::operator *(MaterialSingleOutputPin<T> & node)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4ScalarNode<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialMultVec4ScalarNode<T>::operator *(MaterialOutputPin<Vector4<T>> & pin)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), pin);
    }

    template <typename T>
    MaterialMultVec4Vec4Node<T> & MaterialMultVec4ScalarNode<T>::operator *(MaterialSingleOutputPin<Vector4<T>> & node)
    {
        Material & mat = MaterialSingleOutputPin<Vector4<T>>::m_material;
        return mat.createMultVec4Vec4Node<T>(this->getOutput(), node.getOutput());
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T>::MaterialMultVec4ScalarNode(Material & material) :
        MaterialSingleOutputPin<Vector4<T>>(material, *this),
        m_inputA(*this, "a"),
        m_inputB(*this, "b")
    { }

    template <typename T>
    MaterialMultVec4ScalarNode<T>::MaterialMultVec4ScalarNode(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB) :
        MaterialMultVec4ScalarNode(material)
    {
        m_inputA.connect(pinA);
        m_inputB.connect(pinB);
    }

    template <typename T>
    MaterialMultVec4ScalarNode<T>::~MaterialMultVec4ScalarNode()
    {
        m_inputA.disconnect();
        m_inputB.disconnect();
    }
    */

    // Material output node implementations.
    template<typename T>
    inline MaterialInputPin<T> & MaterialOutputNode<T>::getInput()
    {
        return m_input;
    }

    template<typename T>
    inline MaterialOutputNode<T> & MaterialOutputNode<T>::operator = (MaterialSingleOutputPin<T> & node)
    {
        m_input.connect(node.getOutput());
        return *this;
    }

    template<typename T>
    inline MaterialNodeType MaterialOutputNode<T>::getType() const
    {
        return MaterialNodeType::Output;
    }

    template<typename T>
    inline MaterialDataType MaterialOutputNode<T>::getDataType() const
    {
        return GetDataType<T>::value();
    }

    template<typename T>
    inline MaterialOutputNode<T>::MaterialOutputNode(Material & material) :
        MaterialOutputNodeBase(material),
        m_input(*this, "in")
    { }

    template<typename T>
    template<typename ... Args>
    inline MaterialOutputNode<T>::MaterialOutputNode(Material & material, Args ... args) :
        MaterialOutputNodeBase(material),
        m_input(*this, "in", args...)
    { }

    template<typename T>
    inline MaterialOutputNode<T>::~MaterialOutputNode()
    { }

    template<typename T>
    const MaterialInputPinBase * MaterialOutputNode<T>::getInputBase() const
    {
        return &m_input;
    }


    // GLSL generator implementations.
    template<MaterialNodeType NodeType>
    inline void MaterialGlslGenerator::traverseDeclaration(const MaterialNode * node)
    {

    }

    template<MaterialNodeType NodeType>
    inline void MaterialGlslGenerator::traverseOutput(const MaterialNode * node)
    {
        auto castedNode = reinterpret_cast<const MaterialOutputNodeBase*>(node);

        auto inputPin = castedNode->getInputBase();

        auto connectionBase = inputPin->getConnectionBase();
        if (connectionBase != nullptr)
        {
            traverseNode(&connectionBase->getNode());
        }

        
    }

    template<MaterialNodeType NodeType>
    inline void MaterialGlslGenerator::traverseOperator(const MaterialNode * node)
    {

        switch (node->getType())
        {
            case MaterialNodeType::MultVec4Vec4:
            {
                auto nodeBase = reinterpret_cast<const MaterialMultVec4Vec4NodeBase *>(node);
                auto pinA = nodeBase->getInputABase();
                auto connectionA = pinA->getConnectionBase();

                if (connectionA)
                {
                    traverseNode(&connectionA->getNode());
                }

                auto pinName = pinA->getName();
                int foo = 0;
            }
            break;
            case MaterialNodeType::MultVec4Scalar:
            {
            }
            break;
        default: throw std::runtime_error("Node is not an operator.");    break;
        }
        
        //auto operatorNode * = static_cast<const MaterialOperatorNote*>(node);



       /* auto castedNode = castNodeToOrigin<NodeType>(node);

        auto name = castedNode->getInputA().getName();

        int foo = 0;*/

        //const MaterialDataTypeNode * pBaseClass = reinterpret_cast<const MaterialDataTypeNode*>(&node);

      /* /* switch (node.getType())
        {
            case MaterialNodeType::MultVec4Vec4:    traverseOperator<MaterialNodeType::MultVec4Vec4>(node);     break;
            case MaterialNodeType::MultVec4Scalar:  traverseOperator<MaterialNodeType::MultVec4Scalar>(node);   break;
            default:                                throw std::runtime_error("Node is not an operator.");    break;
        }
        */
       /* switch (pBaseClass->getDataType())
        {
            case MaterialDataType::Boolean:       break;
            case MaterialDataType::Integer:       break;
            case MaterialDataType::Float:        break;
            case MaterialDataType::Vec2Boolean:  break;
            case MaterialDataType::Vec2Integer:  break;
            case MaterialDataType::Vec2Float:     break;
            case MaterialDataType::Vec3Boolean:  break;
            case MaterialDataType::Vec3Integer:  break;
            case MaterialDataType::Vec3Float:     break;
            case MaterialDataType::Vec4Boolean:  break;
            case MaterialDataType::Vec4Integer:  break;
            case MaterialDataType::Vec4Float:    break;
            default: break;
        }*/
    }

    template<MaterialNodeType NodeType>
    auto MaterialGlslGenerator::castNodeToOrigin(const MaterialNode * node)
    {
       /* const MaterialDataTypeNode * pBaseClass = reinterpret_cast<const MaterialDataTypeNode*>(&node);

        switch (node.getType())
        {
            case MaterialNodeType::Scalar:          return castNodeToOrigin_2<MaterialNodeType::Scalar,         MaterialScalarNode<int>>(node);     break;
            case MaterialNodeType::Vec4:            return castNodeToOrigin_2<MaterialNodeType::Vec4,           MaterialVec4Node<int>>(node);          break;
            case MaterialNodeType::Output:          return castNodeToOrigin_2<MaterialNodeType::Output,         MaterialOutputNode<int>>(node);             break;
            case MaterialNodeType::MultVec4Vec4:    return castNodeToOrigin_2<MaterialNodeType::MultVec4Vec4,   MaterialScalarNode<int>>(node);     break;
            case MaterialNodeType::MultVec4Scalar:  return castNodeToOrigin_2<MaterialNodeType::MultVec4Scalar, MaterialScalarNode<int>>(node);   break;
            default:                                throw std::runtime_error("Unknown material node type.");    break;
        }
        */


      //  const MaterialDataTypeNode * pBaseClass = reinterpret_cast<const MaterialDataTypeNode*>(&node);
        /*
        switch (pBaseClass->getDataType())
        {
            case MaterialDataType::Boolean:     return reinterpret_cast<const   break;
            case MaterialDataType::Integer:       break;
            case MaterialDataType::Float:        break;
            case MaterialDataType::Vec2Boolean:  break;
            case MaterialDataType::Vec2Integer:  break;
            case MaterialDataType::Vec2Float:     break;
            case MaterialDataType::Vec3Boolean:  break;
            case MaterialDataType::Vec3Integer:  break;
            case MaterialDataType::Vec3Float:     break;
            case MaterialDataType::Vec4Boolean:  break;
            case MaterialDataType::Vec4Integer:  break;
            case MaterialDataType::Vec4Float:    break;
            default: break;
        }
        */
        throw std::runtime_error("Unkown type.");

        return (int)123;
        //return static_cast<const T<DataType>
    }

   /* template<>
    inline void MaterialGlslGenerator::traverseNodeType<MaterialNodeType::Scalar>(const MaterialNode & node)
    {

    }
    template<>
    inline void MaterialGlslGenerator::traverseNodeType<MaterialNodeType::Vec4>(const MaterialNode & node)
    {

    }
    template<>
    inline void MaterialGlslGenerator::traverseNodeType<MaterialNodeType::MultVec4Vec4>(const MaterialNode & node)
    {

    }
    template<>
    inline void MaterialGlslGenerator::traverseNodeType<MaterialNodeType::MultVec4Scalar>(const MaterialNode & node)
    {

    }
    template<>
    inline void MaterialGlslGenerator::traverseNodeType<MaterialNodeType::Output>(const MaterialNode & node)
    {
        const MaterialOutputNodeBase * base = static_cast<const MaterialOutputNodeBase*>(&node);
        //traverseDataType<
    }*/

    // Material implementations.
    template<typename T, typename ... Args>
    inline MaterialOutputNode<T> & Material::createOutputNode(Args ... args)
    {
        auto node = new MaterialOutputNode<T>(*this, args...);
        m_nodes.insert(node);
        m_outputNodes.push_back(node);
        return *node;
    }

    /*template<typename T, typename ... Args>
    inline MaterialScalarNode<T> & Material::createScalarNode(Args ... args)
    {
        auto node = new MaterialScalarNode<T>(*this, args...);
        m_nodes.insert(node);
        return *node;
    }*/

    template<typename T, typename ... Args>
    inline MaterialVec4Node<T> & Material::createVec4Node(Args ... args)
    {
        auto node = new MaterialVec4Node<T>(*this, args...);
        m_nodes.insert(node);
        return *node;
    }

    /*template<typename T, typename ... Args>
    MaterialMultVec4Vec4Node<T> & Material::createMultVec4Vec4Node(Args ... args)
    {
        auto node = new MaterialMultVec4Vec4Node<T>(*this, args...);
        m_nodes.insert(node);
        return *node;
    }*/
    template<typename T>
    inline MaterialMultVec4Vec4Node<T> & Material::createMultVec4Vec4Node(MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<Vector4<T>> & pinB)
    {
        auto node = new MaterialMultVec4Vec4Node<T>(*this, pinA, pinB);
        m_nodes.insert(node);
        return *node;
    }

    /*template<typename T, typename ... Args>
    MaterialMultVec4ScalarNode<T> & Material::createMultVec4ScalarNode(Args ... args)
    {
        auto node = new MaterialMultVec4ScalarNode<T>(*this, args...);
        m_nodes.insert(node);
        return *node;
    }*/
  /*  template<typename T>
    MaterialMultVec4ScalarNode<T> & Material::createMultVec4ScalarNode(MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB)
    {
        auto node = new MaterialMultVec4ScalarNode<T>(*this, pinA, pinB);
        m_nodes.insert(node);
        return *node;
    }*/

}

