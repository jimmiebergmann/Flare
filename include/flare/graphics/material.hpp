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
    class MaterialInputPinBase;
    class MaterialOutputPinBase;

    template<typename T> class MaterialMultVec4Vec4Node;
    template<typename T> class MaterialMultVec4ScalarNode;
    template<typename T> class MaterialOutputPin;
    template<typename T> class MaterialSingleOutputPin;


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



    class MaterialPinBase
    {

    public:

        virtual ~MaterialPinBase();

        /**
        * Get reference to node owning this pin.
        *
        */
        virtual MaterialNode & getNode() = 0;
        virtual const MaterialNode & getNode() const = 0;

        /**
        * Get name of pin.
        *
        */
        virtual const std::string & getName() const = 0;

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

    };

    /**
    * Material pin base class.
    *
    * @see MaterialInputPin
    * @see MaterialOutputPin
    *
    */
    class MaterialPin : public MaterialPinBase
    {

    public:

        virtual ~MaterialPin();

        /**
        * Get reference to node owning this pin.
        *
        */
        virtual MaterialNode & getNode();
        virtual const MaterialNode & getNode() const;

        /**
        * Get name of pin.
        *
        */
        virtual const std::string & getName() const;

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

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



    class MaterialInputPinBase : public MaterialPin
    {

    public:

        virtual ~MaterialInputPinBase();

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

        virtual MaterialOutputPinBase * getConnectionBase() = 0;
        virtual const MaterialOutputPinBase * getConnectionBase() const = 0;

    protected:

        /**
        * Constructor.
        *
        * @param node   Node owning this pin.
        * @param name   Name of pin.
        *
        */
        MaterialInputPinBase(MaterialNode & node, const std::string & name);

    };


    /**
    * Material input pin.
    *
    * @brief Used in material nodes for connecting incoming data.
    *
    */
    template<typename T>
    class MaterialInputPin : public MaterialInputPinBase
    {

    public:

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const;

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
        MaterialInputPin<T> & operator = (MaterialSingleOutputPin<T> & node);
        MaterialInputPin<T> & operator = (MaterialOutputPin<T> & outputPin);

    private:

        virtual MaterialOutputPinBase * getConnectionBase();
        virtual const MaterialOutputPinBase * getConnectionBase() const;

        T                       m_value;
        MaterialOutputPin<T> *  m_connection;

    };


    class MaterialOutputPinBase : public MaterialPin
    {

    public:

        virtual ~MaterialOutputPinBase();

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

    protected:

        /**
        * Constructor.
        *
        * @param node   Node owning this pin.
        * @param name   Name of pin.
        *
        */
        MaterialOutputPinBase(MaterialNode & node, const std::string & name);

    };


    /**
    * Material output pin.
    *
    * @brief Used in material nodes for returning data.
    *
    */
    template<typename T>
    class MaterialOutputPin : public MaterialOutputPinBase
    {

    public:

        /**
        * Getdata type of pin.
        *
        */
        virtual MaterialDataType getDataType() const;

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
    class MaterialSingleOutputPin
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
        MaterialSingleOutputPin(MaterialNode * node);
        MaterialSingleOutputPin(MaterialNode * node, const std::string & pinName);

        MaterialOutputPin<T> m_output;

    };


    /**
    * Vector4 node.
    *
    */
   /* template<typename T>
    class MaterialScalarNode : public MaterialSingleOutputPin<T>
    {

    public:

        /**
        * Get node type.
        *
        */
        //virtual MaterialNodeType getType() const;

        /**
        * Get X input pin.
        *
        */
     /*   MaterialInputPin<T> & getInput();

    private:

        friend class Material;
        */
        /**
        * Constructor.
        *
        * @param material   Material passed by material class at creation.
        * @param scalar     Setting default values to scalar for all elements.
        *
        */
       /* MaterialScalarNode(Material & material);
        MaterialScalarNode(Material & material, const T scalar);
        ~MaterialScalarNode();

        MaterialInputPin<T> m_input;

    };*/

    
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
    * Base class for Vector4 node.
    *
    */
    class FLARE_API MaterialVec4NodeBase : public MaterialNode, public MaterialDataTypeNode
    {

    public:

        virtual ~MaterialVec4NodeBase();

        virtual MaterialInputPinBase * getInputXBase() = 0;
        virtual const MaterialInputPinBase * getInputXBase() const = 0;
        virtual MaterialInputPinBase * getInputYBase() = 0;
        virtual const MaterialInputPinBase * getInputYBase() const = 0;
        virtual MaterialInputPinBase * getInputZBase() = 0;
        virtual const MaterialInputPinBase * getInputZBase() const = 0;
        virtual MaterialInputPinBase * getInputWBase() = 0;
        virtual const MaterialInputPinBase * getInputWBase() const = 0;
        virtual MaterialOutputPinBase * getOutputBase() = 0;
        virtual const MaterialOutputPinBase * getOutputBase() const = 0;

        /**
        * Get type of node.
        *
        */
        virtual MaterialNodeType getType() const = 0;

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

    protected:

        MaterialVec4NodeBase(Material & material);

    };

    /**
    * Vector4 node.
    *
    */
    template<typename T>
    class MaterialVec4Node : public MaterialVec4NodeBase, public MaterialSingleOutputPin<Vector4<T>>
    {

    public:

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
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputPin<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputPin<Vector4<T>> & node);

        /**
        * Get node type.
        *
        */
        virtual MaterialNodeType getType() const;

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const;

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

        virtual MaterialInputPinBase * getInputXBase();
        virtual const MaterialInputPinBase * getInputXBase() const;
        virtual MaterialInputPinBase * getInputYBase();
        virtual const MaterialInputPinBase * getInputYBase() const;
        virtual MaterialInputPinBase * getInputZBase();
        virtual const MaterialInputPinBase * getInputZBase() const;
        virtual MaterialInputPinBase * getInputWBase();
        virtual const MaterialInputPinBase * getInputWBase() const;
        virtual MaterialOutputPinBase * getOutputBase();
        virtual const MaterialOutputPinBase * getOutputBase() const;

        MaterialInputPin<T> m_inputX;
        MaterialInputPin<T> m_inputY;
        MaterialInputPin<T> m_inputZ;
        MaterialInputPin<T> m_inputW;
        
    };


    /**
    * Base class for vector4 multiplications.
    *
    */
    class FLARE_API MaterialMultVec4Vec4NodeBase : public MaterialNode, public MaterialDataTypeNode
    {

    public:

        virtual ~MaterialMultVec4Vec4NodeBase();

        virtual MaterialInputPinBase * getInputABase() = 0;
        virtual const MaterialInputPinBase * getInputABase() const = 0;
        virtual MaterialInputPinBase * getInputBBase() = 0;
        virtual const MaterialInputPinBase * getInputBBase() const = 0;
        virtual MaterialOutputPinBase * getOutputBase() = 0;
        virtual const MaterialOutputPinBase * getOutputBase() const = 0;

        /**
        * Get type of node.
        *
        */
        virtual MaterialNodeType getType() const = 0;

        /**
        * Get data type.
        *
        */
        virtual MaterialDataType getDataType() const = 0;

    protected:

        MaterialMultVec4Vec4NodeBase(Material & material);

    };

    /**
    * Node for vector4 multiplications.
    *
    */
    template<typename T>
    class MaterialMultVec4Vec4Node : public MaterialMultVec4Vec4NodeBase, public MaterialSingleOutputPin<Vector4<T>>
    {

    public:

        /**
        * Get input nodes.
        *
        */
        MaterialInputPin<Vector4<T>> & getInputA();
        MaterialInputPin<Vector4<T>> & getInputB();

        /**
        * Multiplication operator between nodes.
        *
        */
        MaterialMultVec4ScalarNode<T> & operator *(MaterialOutputPin<T> & pin);
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputPin<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputPin<Vector4<T>> & node);

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

    private:

        friend class Material;

        MaterialMultVec4Vec4Node(Material & material);
        MaterialMultVec4Vec4Node(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<Vector4<T>> & pinB);
        ~MaterialMultVec4Vec4Node();

        virtual MaterialInputPinBase * getInputABase();
        virtual const MaterialInputPinBase * getInputABase() const;
        virtual MaterialInputPinBase * getInputBBase();
        virtual const MaterialInputPinBase * getInputBBase() const;
        virtual MaterialOutputPinBase * getOutputBase();
        virtual const MaterialOutputPinBase * getOutputBase() const;

        MaterialInputPin<Vector4<T>> m_inputA;
        MaterialInputPin<Vector4<T>> m_inputB;
    };

    /**
    * Node for vector4 and scalar multiplications.
    *
    */
    /*template<typename T>
    class MaterialMultVec4ScalarNode : public MaterialDataTypeNode, public MaterialSingleOutputPin<Vector4<T>>
    {

    public:

        /**
        * Get data type.
        *
        */
      /*  virtual MaterialDataType getDataType() const;

        virtual MaterialNodeType getType() const;

        MaterialInputPin<Vector4<T>> & getInputA();
        MaterialInputPin<T> & getInputB();

        MaterialMultVec4ScalarNode<T> & operator *(MaterialOutputPin<T> & pin);
        MaterialMultVec4ScalarNode<T> & operator *(MaterialSingleOutputPin<T> & node);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialOutputPin<Vector4<T>> & pin);
        MaterialMultVec4Vec4Node<T> & operator *(MaterialSingleOutputPin<Vector4<T>> & node);

    private:

        friend class Material;

        MaterialMultVec4ScalarNode(Material & material);
        MaterialMultVec4ScalarNode(Material & material, MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB);
        ~MaterialMultVec4ScalarNode();

        MaterialInputPin<Vector4<T>> m_inputA;
        MaterialInputPin<T> m_inputB;
    };*/
    

    /**
    * Base class of material output nodes.
    *
    */
    class FLARE_API MaterialOutputNodeBase : public MaterialNode, public MaterialDataTypeNode
    {

    public:

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

        /**
        * Get pointer of input base class.
        *
        */
        virtual const MaterialInputPinBase * getInputBase() const = 0;

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
        * Get reference to input.
        *
        */
        MaterialInputPin<T> & getInput();

        /**
        * Connect a node with this output node.
        *
        */
        MaterialOutputNode<T> & operator = (MaterialSingleOutputPin<T> & node);

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

        /**
        * Get pointer of input base class.
        *
        */
        virtual const MaterialInputPinBase * getInputBase() const;

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

        void traverseNode(const MaterialNode * node);

        template<MaterialNodeType NodeType>
        void traverseDeclaration(const MaterialNode * node);

        template<MaterialNodeType NodeType>
        void traverseOutput(const MaterialNode * node);

        template<MaterialNodeType NodeType>
        void traverseOperator(const MaterialNode * node);

        template<MaterialNodeType NodeType>
        auto castNodeToOrigin(const MaterialNode * node);



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
       // template<typename T, typename ... Args>
      //  MaterialScalarNode<T> & createScalarNode(Args ... args);

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
       // template<typename T>
       // MaterialMultVec4ScalarNode<T> & createMultVec4ScalarNode(MaterialOutputPin<Vector4<T>> & pinA, MaterialOutputPin<T> & pinB);

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