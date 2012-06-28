
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

    File Created At:        10/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_TREE_BINARYTREE_H_
#define NC_CORE_TREE_BINARYTREE_H_

#include "BinaryNode.h"
#include "Node.h"

namespace Nc
{
    namespace Graph
    {
        /// Template class designed to manipulate a binary tree structure
        /**
            The Binary tree use by default a BinaryNodeAVL to stock the datas. The BinaryTree could be see as a simple abstraction to use more easily the root node
			
            By default the BinaryTree use a BinaryNodeAVL to define the comportement of the insertion/remove into the tree.
            But you could implement your own Node and Politic to define a specific comportement.

 <div class="title"> Here a sample to use the BinaryTree : </div>
 \code
    #include <Nc/Core/Graph/DotGraph.h>
    #include <Nc/Core/Graph/BinaryTree.h>
    #include <Nc/Core/Utils/Logger.h>

    using namespace Nc;
    using namespace Nc::Graph;

    int main()
    {
        BinaryTree<int> t(100);

        t.Insert(1);
        t.Insert(24);
        t.Insert(3);
        t.Insert(23);
        t.Insert(5);
        t.Insert(7);
        t.Insert(34);
        t.Insert(8);
        t.Insert(3);

        for (BinaryTree<int>::NodeType *n = t.NodeMini(); n != NULL; n = n->Next())
            LOG << n->Data << " ";
        LOG << std::endl;
        for (BinaryTree<int>::NodeType *n = t.NodeMaxi(); n != NULL; n = n->Prev())
            LOG << n->Data << " ";
        LOG << std::endl;

        LOG << "print" << std::endl;
        if (t.Root() != NULL)
            DotGraph<BinaryTree<int>::NodeType >::Save("testBinaryNode.dot", *t.Root());
        return 0;
    }
 \endcode
 <div class="title"> Run the command `dot testBinaryNode.dot -Tpng > result.png` to see the result. </div>

            \sa
            - Node
            - BinaryNodeAVL
            - BinaryNodeAVLPolitic
            - BinaryNodePolitic
            - INodePolitic
        */
        template<typename T, template<typename> class N = BinaryNodeAVL>
        class BinaryTree
        {
            public:
                typedef N<T>      NodeType;

            public:
                BinaryTree()                                        : _root(NULL)                           {}
                BinaryTree(const T &data)                           : _root(new NodeType(data))             {}
                BinaryTree(NodeType *parent)                        : _root(new NodeType(parent))           {}
                BinaryTree(const T &data, NodeType *parent)         : _root(new NodeType(data, parent))     {}
                BinaryTree(const BinaryTree &bt);
                BinaryTree &operator = (const BinaryTree &bt);
                ~BinaryTree()                                       {if (_root != NULL) delete _root;}

                /** \return the root node */
                NodeType        *Root()                             {return _root;}
                /** \return the root node */
                const NodeType  *Root() const                       {return _root;}

                /**
                    Search a key into the tree.
                    \return the node equals to the given key. return NULL if nothing is found
                */
                NodeType        *Search(const T &key);

                /** Insert the given key into the tree */
                void            Insert(const T &key);

                /**
                    Remove the first witch match with the given key
                    \param data, filled with the data of the removed node if not null and if a node has been removed
                    \return true if a node has been removed
                */
                bool            Remove(const T &key, T *data = NULL);

                /** \return the minimal node */
                NodeType        *NodeMini();
                /** \return the maximal node */
                NodeType        *NodeMaxi();

            protected:
                NodeType        *_root;             ///< The root of the tree
        };

        template<typename T, template<typename> class N>
        BinaryTree<T,N>::BinaryTree(const BinaryTree &bt)
        {
            _root = (bt._root != NULL) ? new NodeType(*bt._root) : NULL;
        }

        template<typename T, template<typename> class N>
        BinaryTree<T,N> &BinaryTree<T,N>::operator = (const BinaryTree &bt)
        {
            if (_root != NULL)
                delete _root;
            _root = (bt._root != NULL) ? new NodeType(*bt._root) : NULL;
            return *this;
        }

        template<typename T, template<typename> class N>
        typename BinaryTree<T,N>::NodeType    *BinaryTree<T,N>::Search(const T &key)
        {
            if (_root != NULL)
            {
                NodeType *n = _root->Search(key);
                return ((n) ? n->Data : NULL);
            }
            else
                return NULL;
        }

        template<typename T, template<typename> class N>
        void        BinaryTree<T,N>::Insert(const T &key)
        {
            NodeType *n = new NodeType(key);
            _root = (_root != NULL) ? _root->Insert(n) : n;
        }

        template<typename T, template<typename> class N>
        bool        BinaryTree<T,N>::Remove(const T &key, T *data)
        {
            if (_root != NULL)
            {
                NodeType *n = NULL;
                _root = _root->Remove(key, n);
                if (data != NULL && n != NULL)
                    *data = n->Data;
                if (n != NULL)
                {
                    delete n;
                    return true;
                }
            }
            return false;
        }

        template<typename T, template<typename> class N>
        typename BinaryTree<T,N>::NodeType    *BinaryTree<T,N>::NodeMini()
        {
            if (_root != NULL)
            {
                NodeType *n = _root;
                while (n->Left() != NULL)
                    n = n->Left();
                return n;
            }
            return NULL;
        }

        template<typename T, template<typename> class N>
        typename BinaryTree<T,N>::NodeType    *BinaryTree<T,N>::NodeMaxi()
        {
            if (_root != NULL)
            {
                NodeType *n = _root;
                while (n->Right() != NULL)
                    n = n->Right();
                return n;
            }
            return NULL;
        }
    }
}

#endif
