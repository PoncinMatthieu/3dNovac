
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

    File Created At:        25/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_TREE_BINARYNODE_H_
#define NC_CORE_TREE_BINARYNODE_H_

#include "INodePolitic.h"
#include "Node.h"

namespace Nc
{
    namespace Graph
    {
        /// Politic class to define a binary node, has a left/right node
        template<typename T, class NodeType, class Allocator = Utils::Metaprog::Allocator<NodeType> >
        class BinaryNodePolitic : public INodePolitic<T,NodeType, false>
        {
            public:
                typedef INodePolitic<T,NodeType,false>      NodePolitic;

            public:
                BinaryNodePolitic()                                     : NodePolitic(), _left(NULL), _right(NULL)                {}
                BinaryNodePolitic(const T &data)                        : NodePolitic(data), _left(NULL), _right(NULL)            {}
                BinaryNodePolitic(NodeType *parent)                     : NodePolitic(parent), _left(NULL), _right(NULL)          {}
                BinaryNodePolitic(const T &data, NodeType *parent)      : NodePolitic(data, parent), _left(NULL), _right(NULL)    {}
                BinaryNodePolitic(const BinaryNodePolitic &bn);
                BinaryNodePolitic &operator = (const BinaryNodePolitic &bn);
                ~BinaryNodePolitic();

                /** \return the left node */
                NodeType            *Left()                             {return _left;}
                /** \return the left node */
                const NodeType      *Left() const                       {return _left;}
                /** \return the right node */
                NodeType            *Right()                            {return _right;}
                /** \return the right node */
                const NodeType      *Right() const                      {return _right;}

                /** \return the next node (in ascending order) */
                NodeType            *Next();
                /** \return the next node (in ascending order) */
                const NodeType      *Next() const;
                /** \return the next node (in descending order) */
                NodeType            *Prev();
                /** \return the next node (in descending order) */
                const NodeType      *Prev() const;

                /** \return the height of the node (length of the longest downward path to a leaf) */
                int                 Height() const;

                /** \return true if the node has at least one child */
                bool                HasChilds() const                   {return (_left != NULL || _right != NULL);}
                /** \return the number of child nodes */
                unsigned int        ChildCount() const                  {unsigned int n = (_left) ? 1 : 0; return n + ((_right) ? 1 : 0);}
                /** \return the ieme child */
                NodeType            *Child(unsigned int i);
                /** \return the ieme child */
                const NodeType      *Child(unsigned int i) const;

                /** Extract all data's leafs of the node into the given list */
                void                ExtractLeaf(std::list<T> &leafs) const;
                /** Extract all data's childs of the node into the given list */
                void                ExtractChilds(std::list<T> &childs) const;

                /** \return the node which match with the given key */
                NodeType            *Search(const T &key);
                /**
                    Insert the given node
                    \return the root node
                */
                NodeType            *Insert(NodeType *newNode);
                /**
                    Remove the node which match with the given key
                    \param removedNode will be set to the removed node
                    \return the node which take place insteed of the removedNode
                */
                NodeType            *Remove(const T &key, NodeType *&removedNode);

                // specific structure method
                /** \return the balance factor of the node (the Height of the left node minus the Height of the right node) */
                int                 BalanceFactor() const;
                /** \return true if the node is balanced (BalanceFactor need to be equals to -1, 0 or -1) */
                bool                Balanced() const                    {int n = BalanceFactor(); return (n > -2 && n < 2);}

            protected:
                /** To rebalance the tree, execute a left rotation of the node */
                NodeType            *LeftRotation();
                /** To rebalance the tree, execute a right rotation of the node */
                NodeType            *RightRotation();

            protected:
                NodeType    *_left;         ///< the left node
                NodeType    *_right;        ///< the right node
                Allocator   _alloc;         ///< the allocator which allocate the left and right node
        };

        /** To define a binary search tree using the AVL method */
        template<typename T, class NodeType, class Allocator = Utils::Metaprog::Allocator<NodeType> >
        class BinaryNodeAVLPolitic : public BinaryNodePolitic<T, NodeType, Allocator>
        {
            public:
                typedef BinaryNodePolitic<T, NodeType, Allocator>     NodePolitic;

            public:
                BinaryNodeAVLPolitic()                                     : NodePolitic()              {}
                BinaryNodeAVLPolitic(const T &data)                        : NodePolitic(data)          {}
                BinaryNodeAVLPolitic(NodeType *parent)                     : NodePolitic(parent)        {}
                BinaryNodeAVLPolitic(const T &data, NodeType *parent)      : NodePolitic(data, parent)  {}

                /**
                    Insert the given node into the tree
                    \return the root of the tree (the tree has been balanced so the root could have change)
                */
                NodeType        *Insert(NodeType *newNode);
                /**
                    Remove the first node which match with the given key, and fill the removed node
                    \return the root of the tree (the tree has been balanced so the root could have change)
                */
                NodeType        *Remove(const T &key, NodeType *&removedNode);

            private:
                /**
                    Rebalance the node
                    \return the root tree, because it could changed after a rebalancing.
                */
                NodeType        *Rebalancing();
        };

        /// Template class to define a binary tree structure
        /**
            Use a Politic to define the comportement of the node
            \sa
            - BinatyNodeAVLPolitic
            - BinaryNodePolitic
        */
        template<typename T>
        struct BinaryNodeAVL : public Node<T, BinaryNodeAVLPolitic, BinaryNodeAVL<T> >
        {
            typedef Node<T, BinaryNodeAVLPolitic, BinaryNodeAVL<T> >    Parent;
            NC_UTILS_DEFINE_VISITABLE(Priv::INodeBasePolitic);
            typedef Node<T, BinaryNodeAVLPolitic, BinaryNodeAVL>        NodePolitic;
            typedef BinaryNodeAVL                                       NodeType;

            BinaryNodeAVL()                                    : NodePolitic()               {}
            BinaryNodeAVL(const T &data)                       : NodePolitic(data)           {}
            BinaryNodeAVL(NodeType *parent)                    : NodePolitic(parent)         {}
            BinaryNodeAVL(const T &data, NodeType *parent)     : NodePolitic(data, parent)   {}
        };

        template<typename T, class NodeType, class Allocator>
        BinaryNodePolitic<T,NodeType,Allocator>::BinaryNodePolitic(const BinaryNodePolitic &bn) : NodePolitic(bn)
        {
            if (bn._left != NULL)
            {
                _left = _alloc.Allocate(*bn._left);
                _left->_parent = static_cast<NodeType*>(this);
            }
            else
                _left = NULL;
            if (bn._right != NULL)
            {
                _right = _alloc.Allocate(*bn._right);
                _right->_parent = static_cast<NodeType*>(this);
            }
            else
                _right = NULL;
        }

        template<typename T, class NodeType, class Allocator>
        BinaryNodePolitic<T,NodeType,Allocator> &BinaryNodePolitic<T,NodeType,Allocator>::operator = (const BinaryNodePolitic &bn)
        {
            if (_left != NULL)
                _alloc.Deallocate(_left);
            if (_right != NULL)
                _alloc.Deallocate(_right);
            if (bn._left != NULL)
            {
                _left = _alloc.Allocate(*bn._left);
                _left->_parent = static_cast<NodeType*>(this);
            }
            else
                _left = NULL;
            if (bn._right != NULL)
            {
                _right = _alloc.Allocate(*bn._right);
                _right->_parent = static_cast<NodeType*>(this);
            }
            else
                _right = NULL;
            return *this;
        }

        template<typename T, class NodeType, class Allocator>
        BinaryNodePolitic<T,NodeType,Allocator>::~BinaryNodePolitic()
        {
            if (_left != NULL)
                _alloc.Deallocate(_left);
            if (_right != NULL)
                _alloc.Deallocate(_right);
        }

        template<typename T, class NodeType, class Allocator>
        NodeType       *BinaryNodePolitic<T,NodeType,Allocator>::Next()
        {
            if (_right != NULL)
            {
                NodeType *n = _right;
                while (n->_left != NULL)
                    n = n->_left;
                return n;
            }
            for (NodeType *n = static_cast<NodeType*>(this); n->_parent != NULL; n = n->_parent)
                if (n->_parent->_left == n)
                    return n->_parent;
            return NULL;
        }

        template<typename T, class NodeType, class Allocator>
        const NodeType      *BinaryNodePolitic<T,NodeType,Allocator>::Next() const
        {
            if (_right != NULL)
            {
                NodeType *n = _right;
                while (n->_left != NULL)
                    n = n->_left;
                return n;
            }
            for (const NodeType *n = static_cast<const NodeType*>(this); n->_parent != NULL; n = n->_parent)
                if (n->_parent->_left == n)
                    return n->_parent;
            return NULL;
        }

        template<typename T, class NodeType, class Allocator>
        NodeType            *BinaryNodePolitic<T,NodeType,Allocator>::Prev()
        {
            if (_left != NULL)
            {
                NodeType *n = _left;
                while (n->_right != NULL)
                    n = n->_right;
                return n;
            }
            for (NodeType *n = static_cast<NodeType*>(this); n->_parent != NULL; n = n->_parent)
                if (n->_parent->_right == n)
                    return n->_parent;
            return NULL;
        }

        template<typename T, class NodeType, class Allocator>
        const NodeType      *BinaryNodePolitic<T,NodeType,Allocator>::Prev() const
        {
            if (_left != NULL)
            {
                NodeType *n = _left;
                while (n->_right != NULL)
                    n = n->_right;
                return n;
            }
            for (const NodeType *n = static_cast<const NodeType*>(this); n->_parent != NULL; n = n->_parent)
                if (n->_parent->_right == n)
                    return n->_parent;
            return NULL;
        }

        template<typename T, class NodeType, class Allocator>
        int        BinaryNodePolitic<T,NodeType,Allocator>::Height() const
        {
            int n1 = 0, n2 = 0;
            if (_left != NULL)
                n1 = 1 + _left->Height();
            if (_right != NULL)
                n2 = 1 + _right->Height();
            return ((n1 > n2) ? n1 : n2);
        }

        template<typename T, class NodeType, class Allocator>
        NodeType        *BinaryNodePolitic<T,NodeType,Allocator>::Child(unsigned int i)
        {
            if (i == 0)
            {
                if (_left)
                    return _left;
                else if (_right)
                    return _right;
            }
            else if (i == 1 && _right)
                return _right;
            throw Utils::Exception(NodePolitic::NodeTypeToString(), "Out of range");
        }

        template<typename T, class NodeType, class Allocator>
        const NodeType  *BinaryNodePolitic<T,NodeType,Allocator>::Child(unsigned int i) const
        {
            if (i == 0)
            {
                if (_left)
                    return _left;
                else if (_right)
                    return _right;
            }
            else if (i == 1 && _right)
                return _right;
            throw Utils::Exception(NodePolitic::NodeTypeToString(), "Out of range");
        }

        template<typename T, class NodeType, class Allocator>
        void            BinaryNodePolitic<T,NodeType,Allocator>::ExtractLeaf(std::list<T> &leafs) const
        {
            // left to right
            if (!HasChilds())
                leafs.push_back(NodePolitic::Data);
            if (_left != NULL)
                _left->ExtractLeaf(leafs);
            if (_right != NULL)
                _right->ExtractLeaf(leafs);
        }

        template<typename T, class NodeType, class Allocator>
        void            BinaryNodePolitic<T,NodeType,Allocator>::ExtractChilds(std::list<T> &childs) const
        {
            // left first, next this, and finally right
            if (_left != NULL)
                _left->ExtractChilds(childs);
            childs.push_back(NodePolitic::Data);
            if (_right != NULL)
                _right->ExtractChilds(childs);
        }

        template<typename T, class NodeType, class Allocator>
        int    BinaryNodePolitic<T,NodeType,Allocator>::BalanceFactor() const
        {
            int n = (_left != NULL) ? (1 + _left->Height()) : 0;
            return (n - ((_right != NULL) ? (1 + _right->Height()) : 0));
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodePolitic<T,NodeType,Allocator>::Search(const T &key)
        {
            if (key == NodePolitic::Data)
                return static_cast<NodeType*>(this);
            else if (key < NodePolitic::Data)
                return (_left != NULL) ? _left->Search(key) : NULL;
            else
                return (_right != NULL) ? _right->Search(key) : NULL;
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodePolitic<T,NodeType,Allocator>::Insert(NodeType *newNode)
        {
            if (newNode->Data <= NodePolitic::Data)
            {
                if (!_left) // leaf insertion
                {
                    newNode->_parent = static_cast<NodeType*>(this);
                    _left = newNode;
                }
                else // next
                    static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(_left)->Insert(newNode);
            }
            else
            {
                if (!_right) // leaf insertion
                {
                    newNode->_parent = static_cast<NodeType*>(this);
                    _right = newNode;
                }
                else // next
                    static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(_right)->Insert(newNode);
            }
            return static_cast<NodeType*>(this);
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodePolitic<T,NodeType,Allocator>::Remove(const T &key, NodeType *&removedNode)
        {
            if (key == NodePolitic::Data)
            {
                NodeType *replacementNode = NULL;
                removedNode = static_cast<NodeType*>(this);

                if (removedNode->IsLeaf()) // if we have a leaf
                {
                    if (removedNode->_parent != NULL)
                    {
                        if (removedNode->_parent->_left == removedNode)
                            removedNode->_parent->_left = NULL;
                        else
                            removedNode->_parent->_right = NULL;
                    }
                    replacementNode = removedNode->_parent;
                }
                else
                {
                    // search for the replacement node (largest node of the left subtree)
                    if (_left != NULL)
                    {
                        replacementNode = _left;
                        while (replacementNode->_right != NULL)
                            replacementNode = replacementNode->_right;

                        // replace that node with the removed node
                        if (removedNode->_parent != NULL)
                        {
                            if (removedNode->_parent->_left == removedNode)
                                removedNode->_parent->_left = replacementNode;
                            else
                                removedNode->_parent->_right = replacementNode;
                        }
                        if (replacementNode->_parent != NULL)
                        {
                            if (replacementNode->_parent->_left == replacementNode)
                                replacementNode->_parent->_left = NULL;
                            else
                                replacementNode->_parent->_right = NULL;
                        }
                        replacementNode->_parent = removedNode->_parent;
                        replacementNode->_right = removedNode->_right;
                        if (replacementNode->_right != NULL)
                            replacementNode->_right->_parent = replacementNode;

                        // insert the left node in the replacement node
                        if (removedNode->_left != NULL)
                        {
                            NodeType *n = removedNode->_left;
                            n->_parent = NULL;
                            static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(replacementNode)->Insert(n);
                            replacementNode = n;
                        }
                    }
                    else
                    {
                        replacementNode = _right;
                        while (replacementNode->_left != NULL)
                            replacementNode = replacementNode->_left;

                        // replace that node with the removed node
                        if (removedNode->_parent != NULL)
                        {
                            if (removedNode->_parent->_left == removedNode)
                                removedNode->_parent->_left = replacementNode;
                            else
                                removedNode->_parent->_right = replacementNode;
                        }
                        if (replacementNode->_parent != NULL)
                        {
                            if (replacementNode->_parent->_left == replacementNode)
                                replacementNode->_parent->_left = NULL;
                            else
                                replacementNode->_parent->_right = NULL;
                        }
                        replacementNode->_parent = removedNode->_parent;
                        replacementNode->_left = removedNode->_left;
                        if (replacementNode->_left != NULL)
                            replacementNode->_left->_parent = replacementNode;

                        // insert the left node in the replacement node
                        if (removedNode->_right != NULL)
                        {
                            NodeType *n = removedNode->_right;
                            n->_parent = NULL;
                            static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(replacementNode)->Insert(n);
                            replacementNode = n;
                        }
                    }
                }
                removedNode->_parent = NULL;
                removedNode->_left = NULL;
                removedNode->_right = NULL;
                return replacementNode;
            }
            else if (key < NodePolitic::Data)   // next
                return (_left != NULL) ? static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(_left)->Remove(key, removedNode) : NULL;
            else // next
                return (_right != NULL) ? static_cast<BinaryNodePolitic<T,NodeType,Allocator>*>(_right)->Remove(key, removedNode) : NULL;
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodePolitic<T,NodeType,Allocator>::LeftRotation()
        {
            NodeType *newRoot = _right;
            if (newRoot == NULL)
                return static_cast<NodeType*>(this);

            // set the parent of the new root
            if (NodePolitic::_parent != NULL)
            {
                if (NodePolitic::_parent->_left == static_cast<NodeType*>(this))
                    NodePolitic::_parent->_left = newRoot;
                else if (NodePolitic::_parent->_right == static_cast<NodeType*>(this))
                    NodePolitic::_parent->_right = newRoot;
            }
            newRoot->_parent = NodePolitic::_parent;
            NodePolitic::_parent = newRoot;

            // set the right child of the last root
            _right = newRoot->_left;
            if (newRoot->_left != NULL)
                newRoot->_left->_parent = static_cast<NodeType*>(this);

            // set the left child of the new root
            newRoot->_left = static_cast<NodeType*>(this);
            newRoot->_left->_parent = newRoot;
            return newRoot;
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodePolitic<T,NodeType,Allocator>::RightRotation()
        {
            NodeType *newRoot = _left;
            if (newRoot == NULL)
                return static_cast<NodeType*>(this);

            // set the parent of the new root
            if (NodePolitic::_parent != NULL)
            {
                if (NodePolitic::_parent->_left == static_cast<NodeType*>(this))
                    NodePolitic::_parent->_left = newRoot;
                else if (NodePolitic::_parent->_right == static_cast<NodeType*>(this))
                    NodePolitic::_parent->_right = newRoot;
            }
            newRoot->_parent = NodePolitic::_parent;
            NodePolitic::_parent = newRoot;

            // set the left child of the last root
            _left = newRoot->_right;
            if (newRoot->_right != NULL)
                newRoot->_right->_parent = static_cast<NodeType*>(this);

            // set the left child of the new root
            newRoot->_right = static_cast<NodeType*>(this);
            newRoot->_right->_parent = newRoot;
            return newRoot;
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodeAVLPolitic<T,NodeType,Allocator>::Insert(NodeType *newNode)
        {
            // insert
            NodePolitic::Insert(newNode);

            // Rebalancing
            return newNode->Rebalancing();
        }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodeAVLPolitic<T,NodeType,Allocator>::Rebalancing()
        {
            NodeType    *newRoot = static_cast<NodeType*>(this);
            int         f = NodePolitic::BalanceFactor();

            if (f > 1)
                newRoot = NodePolitic::RightRotation();
            else if (f < -1)
                newRoot = NodePolitic::LeftRotation();
            if (newRoot->_parent != NULL)
                return newRoot->_parent->Rebalancing();
            return newRoot;
       }

        template<typename T, class NodeType, class Allocator>
        NodeType    *BinaryNodeAVLPolitic<T,NodeType,Allocator>::Remove(const T &key, NodeType *&removedNode)
        {
            // remove
            removedNode = NULL;
            NodeType *node = NodePolitic::Remove(key, removedNode);
            if (removedNode == NULL)
                return static_cast<NodeType*>(this);

            // rebalancing
            return  (node != NULL) ? node->Rebalancing() : NULL;
        }
    }
}

#endif // NC_CORE_TREE_BINARYNODE_H_
