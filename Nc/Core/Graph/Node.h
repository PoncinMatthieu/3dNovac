
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        13/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_TREE_INODE_H_
#define NC_CORE_TREE_INODE_H_

#include <list>
#include "../Define.h"
#include "../Utils/Metaprog.h"
#include "../Utils/Visitable.h"
#include "INodePolitic.h"

namespace Nc
{
    /// Define some usefull abstract and implementation of graph/tree structure
    namespace Graph
    {
        /// Abstract class used to define a node by using a Politic \p P, a \p NodeType and an \p Allocator
        template<typename T, template<typename, class, class> class P, class NodeType, class Allocator = Utils::Metaprog::Allocator<NodeType> >
        class AbstractNode : public P<T,NodeType,Allocator>
        {
            public:
                typedef P<T,NodeType,Allocator>             Parent;
                NC_UTILS_DEFINE_VISITABLE(Priv::INodeBasePolitic);
                typedef P<T,NodeType,Allocator>             NodePolitic;

            public:
                AbstractNode()                                  : NodePolitic()               {}
                AbstractNode(NodeType *parent)                  : NodePolitic(parent)         {}

                /** \return true if the node is root (has no parent) */
                bool            IsRoot() const                  {return NodePolitic::IsRoot();}

                /** \return true if the node is a leaf (do not have any children) */
                bool            IsLeaf() const                  {return !NodePolitic::HasChilds();}
                /** \return true if the node is an internal node (has at least one child) */
                bool            IsInner() const                 {return NodePolitic::HasChilds();}

                /** \return true if the node has at least one child */
                bool            HasChilds() const               {return NodePolitic::HasChilds();}
                /** \return the number of child nodes */
                unsigned int    ChildCount() const              {return NodePolitic::ChildCount();}
                /** \return the ieme child */
                NodeType        *Child(unsigned int i)          {return NodePolitic::Child(i);}
                /** \return the ieme child */
                const NodeType  *Child(unsigned int i) const    {return NodePolitic::Child(i);}
        };

        /// Template class Node used to define and manipulate a tree/graph structure using a politic
        /**
            Basically the node use the same terminology of The Graph Theory.
            The node contained a data of type T and use the Politic P to define it's comportement.

            You couldn't create directly a node with this class.
            But you could use a BinaryNode or the specialisation of the node with a NodeType of Utils::Metaprog::Nop

            \sa
            - BinaryNode
            - Node<T,P,Utils::Metaprog::Nop>
            - BinaryNodeAVLPolitic
            - ListNodePolitic
            - NNodePolitic
        */
        template<typename T, template<typename, class, class> class P = ListNodePolitic, class NodeType = Utils::Metaprog::Nop>
        class Node : public AbstractNode<T,P,NodeType>
        {
            public:
                typedef AbstractNode<T,P,NodeType>      Parent;
                NC_UTILS_DEFINE_VISITABLE(Priv::INodeBasePolitic);
                typedef AbstractNode<T,P,NodeType>      NodePolitic;

            public:
                Node()                                  : NodePolitic()               {}
                Node(NodeType *parent)                  : NodePolitic(parent)         {}
        };

        /// Specialisation of Node
        /**
            Used to define and manipulate a tree/graph structure using a politic and the type of data which would be stored with the data structure.
 <div class="title"> To use this specialisation directly, you can create your node like this: </div>
 \code
    #include <Nc/Core/Utils/Logger.h>
    #include <Nc/Core/Graph/DotGraph.h>
    #include <Nc/Core/Graph/NNodePolitic.h>

    using namespace Nc;
    using namespace Nc::Graph;

    void main()
    {
        Node<int>   node(1);

        node.AddChild(new Node<int>(10));
        node.AddChild(new Node<int>(10));
        node.AddChild(new Node<int>(10));
        node.AddChild(new Node<int>(0));
        node.AddChild(new Node<int>(120));
        node.AddChild(new Node<int>(120));
        node.AddChild(new Node<int>(1430));

        node.Child(2)->AddChild(new Node<int>(345));

        LOG << "print" << std::endl;
        DotGraph<Node<int, ListNodePolitic> >::Save("testListNode.dot", node);
    }
 \endcode

 <div class="title"> run the command `dot testListNode.dot -Tpng > result.png` to see the result. </div>

        */
        template<typename T, template<typename, class, class> class P>
        class Node<T,P,Utils::Metaprog::Nop> : public AbstractNode<T, P, Node<T,P,Utils::Metaprog::Nop> >
        {
            public:
                typedef AbstractNode<T, P, Node<T,P,Utils::Metaprog::Nop> >     Parent;
                NC_UTILS_DEFINE_VISITABLE(Priv::INodeBasePolitic);
                typedef AbstractNode<T, P, Node<T,P,Utils::Metaprog::Nop> >     NodePolitic;
                typedef Node                                                    NodeType;

            public:
                Node()                                  : NodePolitic()               {}
                Node(NodeType *parent)                  : NodePolitic(parent)         {}
        };
    }
}


#endif
