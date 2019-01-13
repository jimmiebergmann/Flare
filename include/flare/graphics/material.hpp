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

#ifndef FLARE_GRAPHICS_MATERIAL_HPP
#define FLARE_GRAPHICS_MATERIAL_HPP

#include "flare/build.hpp"
#include "flare/math/vector.hpp"
#include <functional>
#include <set>
#include <map>
#include <list>


namespace Flare
{

    // Forward declarations.
    class Material;
    class MaterialNode;

    template<typename T> class MaterialMultVec4Vec4Node;
    template<typename T> class MaterialMultVec4ScalarNode;
    template<typename T> class MaterialOutputPin;
    template<typename T> class MaterialSingleOutputNode;


    /**
    * Enumerator of supported pin data types.
    *
    */
    enum class MaterialDataType
    {
        Boolean,
        Integer,
        Float,

        Vec2Boolean,
        Vec2Integer,
        Vec2Float,

        Vec3Boolean,
        Vec3Integer,
        Vec3Float,

        Vec4Boolean,
        Vec4Integer,
        Vec4Float
    };


    /**
    * Enumerator of different node types.
    *
    */
    enum class MaterialNodeType
    {
        Scalar,
        Vec4,
        MultVec4Vec4,
        MultVec4Scalar,
        Output
    };

    template <typename T>
    struct GetDataType
    {

        static MaterialDataType value();

    };


    /**
    * Material pin base class.
    *
    * @see MaterialInputPin
    * @see MaterialOutputPin
    *
    */
    template<typename T>
    class MaterialPin
    {

    public:

        /**
        * Get reference to node owning this pin.
        *
        */
        MaterialNode & getNode();
        const MaterialNode & getNode() const;

        /**
        * Get name of pin.
        *
        */
        const std::string & getName() const;

        /**
        * Getdata type of pin.
        *
        */
        MaterialDataType getDataType() const;

    protected:

        /**
        * Constructor.
        *
        * @param node   Node owning this pin.
        * @param name   Name of pin.
        *
        */
        MaterialPin(MaterialNode & node, const std::string & name);

    private:

        /**
        * Deleted copy constructor.
        *
        */
        MaterialPin(const MaterialPin & copy) = delete;

        MaterialNode &  m_node;
        std::string     m_name;

    };


    /**
    * Material input pin.
    *
    * @brief Used in material nodes for connecting incoming data.
    *
    */
    template<typename T>
    class MaterialInputPin : public MaterialPin<T>
    {

    public:

        friend class MaterialOutputPin<T>;

        /**
        * Constructor.
        *
        * Value is set to false or 0 by default if no value is passed.
        *
        @param node     Reference to owner of this input pin.
        @param name     Name of pin.
        @param value    Value of node.
        *
        */
        MaterialInputPin(MaterialNode & node, const std::string & name);
        template<typename ... Args>
        MaterialInputPin(MaterialNode & node, const std::string & name, Args ... value);

        /**
        * Destructor.
        *
        */
        ~MaterialInputPin();

        /**
        * Get pin value.
        *
        */
        const T & getValue() const;

        /**
        * Set pin value.
        *
        */
        void setValue(const T & value);

        /**
        * Get connected output pin of this input pin.
        *
        * @return nullptr if pin is disconnected, else pointer to output pin with matching template.
        *
        */
        MaterialOutputPin<T> * getConnection();
        const MaterialOutputPin<T> * getConnection() const;

        /**
        * Connect this pin with an output pin.
        *
        */
        void connect(MaterialOutputPin<T> & output);

        /**
        * Disconnect from any output pin.
        *
        */
        void disconnect();

        /**
        * Connect a node with this output node.
        *
        */
        MaterialInputPin<T> & operator = (MaterialSingleOutputNode<T> & node);
        MaterialInputPin<T> & operator = (MaterialOutputPin<T> & outputPin);

    private:

        T                       m_value;
        MaterialOutputPin<T> *  m_connection;

    };


    /**
    * Material output pin.
    *
    * @brief Used in material nodes for returning data.
    *
    */
    template<typename T>
    class MaterialOutputPin : public MaterialPin<T>
    {

    public:

        friend class MaterialInputPin<T>;

        /**
        * Constructor.
        *
        @param node     Reference to owner of this output pin.
        @param name     Name of pin.
        *
        */
        MaterialOutputPin(MaterialNode & node, const std::string & name);

        /**
        * Destructor.
        *
        */
        ~MaterialOutputPin();

        /**
        * Connect this pin with an input pin.
        *
        */
        void connect(MaterialInputPin<T> & input);

        /**
        * Disconnect from given input pin.
        *
        */
        void disconnect(MaterialInputPin<T> & input);

        /**
        * Disconnect from all input pins.
        *
        */
        void disconnectAll();

    private:

        std::set<MaterialInputPin<T> *> m_connections;

    };

    /**
    * Base class of material nodes.
    *
    */
    class FLARE_API MaterialNode
    {

    public:

        /**
        * Get node type.
        *
        */
        virtual MaterialNodeType getType() const = 0;

        /**
        * Get reference to material owning this node.
        *
        */
        Material & getMaterial() const;

    protected:

        friend class Material;

        /**
        * Constructor.
        *
        * @param material   Material owning this node, passed by inherited class.
        *
        */
        MaterialNode(Material & material);

        /**
        * Virtual destructor.
        *
        */
        virtual ~MaterialNode();

        Material & m_material;

    private:

        /**
        * Deleted copy constructor.
        *
        */
        MaterialNode(const MaterialNode & copy) = delete;

    };


    /**
    * Helper class for single output pin nodes.
    *
    */
    template <typename T>
    class MaterialSingleOutputNode : public MaterialNode
    {

    public:

        /**
        * Get node type.
        *
        */
        virtual MaterialNodeType getType() const = 0;

        /**
        * Get output pin.
        *
        */
        MaterialOutputPin<T> & getOutput();
        const MaterialOutputPin<T> & getOutput() const;

    protected:

        /**
        * Constructor.
        *
        * @param material   Material passed by inheriting class.
        * @param node       Reference to node owning the output pin.
        * @param pinName    Name of output pin.
        *
        */
        MaterialSingleOutputNode(Material & material, MaterialNode & node);
        MaterialSingleOutputNode(Material & material, MaterialNode & node, const std::string & pinName);

        MaterialOutputPin<T> m_output;

    };


    /**
    * Vector4 node.
    *
    */
    template<typename T>
    class MaterialScalarNode : public MaterialSingleOutputNode<T>
    {

    public:

        /**
        * Get node type.
        *
        */
        virtual MaterialNodeType getType() const;

        /**
        * Get X input pin.
        *
        */
        MaterialInputPin<T> & getInput();

    private:

        friend class Material;

        /**
        * Constructor.
        *
        * @param material   Material passed by material class at creation.
        * @param scalar     Setting default values to scalar for all elements.
        *
        */
        MaterialScalarNode(Material & material);
        MaterialScalarNode(Material & material, const T scalar);
        ~MaterialScalarNode();

        MaterialInputPin<T> m_input;

    };

    
    /**
    ** Base class for all nodes consisting of 1 data type.
    *
    */
    class FLARE_API MaterialDataTypeNode
    {

    public:

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

    };


    /**
    * Vector4 node.
    *
    */
    template<typename T>
    class MaterialVec4Node : public MaterialDataTypeNode, public MaterialSingleOutputNode<Vector4<T>>
    {

    public:

        virtual MaterialDataType getDataType() const;

        /**
        * Get node type.
        *
        */
        virtual MaterialNodeType getType() const;

        /**
        * Get X input pin.
        *
        */
        MaterialInputPin<T> & getInputX();

        /**
        * Get Y input pin.
        *
        */
        MaterialInputPin<T> & getInputY();

        /**
        * Get Z input pin.
        *
        */
        MaterialInputPin<T> & getInputZ();

        /**
        * Get W input pin.
        *
        */
        MaterialInputPin<T> & getInputW();

        /**
        * Multiplication operator between nodes.
        *
        */
        MaterialMultVec4ScalarNode<T> & operator *(MaterialOutputPin<T> & pin);
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputNode<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputNode<Vector4<T>> & node);

    private:

        friend class Material;

        /**
        * Constructor.
        *
        * @param material   Material passed by material class at creation.
        * @param scalar     Setting default values to scalar for all elements.
        *
        */
        MaterialVec4Node(Material & material);
        MaterialVec4Node(Material & material, const T scalar);
        MaterialVec4Node(Material & material, const T x, const T y, const T z, const T w);
        ~MaterialVec4Node();

        MaterialInputPin<T> m_inputX;
        MaterialInputPin<T> m_inputY;
        MaterialInputPin<T> m_inputZ;
        MaterialInputPin<T> m_inputW;
        
    };


    /**
    * Node for vector4 multiplications.
    *
    */
    template<typename T>
    class MaterialMultVec4Vec4Node : public MaterialDataTypeNode, public MaterialSingleOutputNode<Vector4<T>>
    {

    public:

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const;

        virtual MaterialNodeType getType() const;

        MaterialInputPin<Vector4<T>> & getInputA();
        MaterialInputPin<Vector4<T>> & getInputB();

        /**
        * Multiplication operator between nodes.
        *
        */
        MaterialMultVec4ScalarNode<T> & operator *(MaterialOutputPin<T> & pin);
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputNode<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputNode<Vector4<T>> & node);

    private:

        friend class Material;

        MaterialMultVec4Vec4Node(Material & material);
        MaterialMultVec4Vec4Node(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<Vector4<T>> & pinB);
        ~MaterialMultVec4Vec4Node();

        MaterialInputPin<Vector4<T>> m_inputA;
        MaterialInputPin<Vector4<T>> m_inputB;
    };

    /**
    * Node for vector4 and scalar multiplications.
    *
    */
    template<typename T>
    class MaterialMultVec4ScalarNode : public MaterialDataTypeNode, public MaterialSingleOutputNode<Vector4<T>>
    {

    public:

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const;

        virtual MaterialNodeType getType() const;

        MaterialInputPin<Vector4<T>> & getInputA();
        MaterialInputPin<T> & getInputB();

        MaterialMultVec4ScalarNode<T> & operator *(MaterialOutputPin<T> & pin);
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputNode<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputNode<Vector4<T>> & node);

    private:

        friend class Material;

        MaterialMultVec4ScalarNode(Material & material);
        MaterialMultVec4ScalarNode(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB);
        ~MaterialMultVec4ScalarNode();

        MaterialInputPin<Vector4<T>> m_inputA;
        MaterialInputPin<T> m_inputB;
    };
    

    /**
    * Base class of material output nodes.
    *
    */
    class FLARE_API MaterialOutputNodeBase : public MaterialNode
    {

    public:

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

        /**
        * Constructor.
        *
        * @param material   Material passed by MaterialOutputNode.
        *
        */
        MaterialOutputNodeBase(Material & material);

        /**
        * Virtual destructor.
        *
        */
        virtual ~MaterialOutputNodeBase();

    };

    /**
    * Material output node class.
    *
    * Output nodes represents result of shader programs.
    * At least one output not is required in order to generate shader source code.
    *
    */
    template<typename T>
    class MaterialOutputNode : public MaterialOutputNodeBase
    {

    public:

        /**
        * Get type of node.
        *
        */
        virtual MaterialNodeType getType() const;

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const;

        /**
        * Get reference to input.
        *
        */
        MaterialInputPin<T> & getInput();

        /**
        * Connect a node with this output node.
        *
        */
        MaterialOutputNode<T> & operator = (MaterialSingleOutputNode<T> & node);

    private:

        friend class Material;

        /**
        * Constructor.
        *
        * @param material   Material passed by material class at creation.
        *
        */
        MaterialOutputNode(Material & material);
        template<typename ... Args>
        MaterialOutputNode(Material & material, Args ... args);

        /**
        * Destructor
        *
        */
        ~MaterialOutputNode();

        MaterialInputPin<T> m_input;

    };


    /**
    * Helper class, used for generating GLSL source code.
    */
    class FLARE_API MaterialGlslGenerator
    {
    public:

        void run(const Material & material, std::string & source);

    private:

        void traverseOutputNodes(const std::list<MaterialOutputNodeBase *> & outputNodes);

        void traverseNode(const MaterialNode & node);

        template<MaterialNodeType NodeType>
        void traverseNodeType(const MaterialNode & node);

        template<MaterialDataType DataType, typename T>
        void traverseDataType(const MaterialNode & node)
        {

        }

        template<MaterialDataType DataType>
        auto test();


        std::string getVarAsString(MaterialDataType dataType);

        std::map<std::string, const MaterialOutputNodeBase*> m_outputNodes;
        std::set<const MaterialNode*> m_traversedNodes;

    };

    template<>
    inline auto MaterialGlslGenerator::test<MaterialDataType::Float>()
    {
        float a = 1.0f;
        return a;
    }

    template<>
    inline auto MaterialGlslGenerator::test<MaterialDataType::Integer>()
    {
        int a = 1;
        return a;
    }

    /**
    * Material class.
    *
    * @breif The material class is used for constructing shader source files,
    *        by connecting different types of material nodes.
    *        This class is responsible of deallocating all created nodes.
    *
    */
    class FLARE_API Material
    {

    public:

        /**
        * Constructor.
        *
        * @breif Deallocated all remaining nodes that hasnät been deleted via deleteNode(T & node).
        *
        */
        Material();

        /**
        * Constructor.
        *
        */
        ~Material();

        /**
        * Creates and allocates an output node.
        *
        * @param args   Parameters of MaterialOutputVec4Node constructor.
        *
        * @see MaterialOutputVec4Node
        *
        * @return Reference to new node.
        *
        */
        template<typename T, typename ... Args>
        MaterialOutputNode<T> & createOutputNode(Args ... args);

        /**
        * @ see createOutputVec4Node
        *
        */
        template<typename T, typename ... Args>
        MaterialScalarNode<T> & createScalarNode(Args ... args);

        /**
        * @ see createOutputVec4Node
        *
        */
        template<typename T, typename ... Args>
        MaterialVec4Node<T> & createVec4Node(Args ... args);

        /**
        * @ see createOutputVec4Node
        *
        */
       /* template<typename T, typename ... Args>
        MaterialMultVec4Vec4Node<T> & createMultVec4Vec4Node(Args ... args);*/
        template<typename T>
        MaterialMultVec4Vec4Node<T> & createMultVec4Vec4Node(MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<Vector4<T>> & pinB);

        /**
        * @ see createOutputVec4Node
        *
        */
        /*template<typename T, typename ... Args>
        MaterialMultVec4ScalarNode<T> & createMultVec4ScalarNode(Args ... args);*/
        template<typename T>
        MaterialMultVec4ScalarNode<T> & createMultVec4ScalarNode(MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB);

        /**
        * Deletes passed node. Any reference to the node becomes invalid and all connections to the node are disconnected.
        *
        * @param node   Reference to node.
        *
        */
        void deleteNode(MaterialNode & node);


        const std::set<MaterialNode *> & getNodes() const;
        const std::list<MaterialOutputNodeBase *> & getOutputNodes() const;

        /**
        * Passed function is called for every single node of this material, including output nodes.
        * Nodes are passed to the passed function's parameter.
        *
        */
        void forEachNode(std::function<void(MaterialNode &)> func);
        
        /**
        * Passed function is called for every single output node of this material.
        * Nodes are passed to the passed function's argument list.
        *
        */
        void forEachOutputNode(std::function<void(MaterialNode &)> func);

        /**
        * Tempory function for debugging the node graph.
        *
        */
        void debugPrint();

        void generateGlsl(std::string & source);

    private:

        Material(const Material & copy) = delete;

        std::set<MaterialNode *> m_nodes;
        std::list<MaterialOutputNodeBase *> m_outputNodes;

    };
  
}

#include "material.inl"

#endif