
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
        /// Template class designed to manipulate a binary tree structure.
        /**
            The Binary tree use by default a BinaryNodeAVL to stock the datas. The BinaryTree could be seen as a simple abstraction to use more easily the root node.

            By default the BinaryTree use a BinaryNodeAVL to define the comportement of the insertion/remove into the tree.
            But you could implement your own Node and Politic to define a specific comportement.

            <div class="title"> Here is a sample to show how to use the BinaryTree : </div>
                \p Examples/BinaryTree.cpp
                \include BinaryTree.cpp

                Run the command `dot testBinaryNode.dot -Tpng > result.png` to see the result printed out by the DotGraph into an image.

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
                BinaryTree()                                        : _root(NULL), _first(NULL), _last(NULL)     {}
                BinaryTree(const BinaryTree &bt);
                BinaryTree &operator = (const BinaryTree &bt);
                ~BinaryTree()                                       {Clear();}

                /** Remove every node in the tree (delete the root, every nodes are deleted by its parent). */
                void            Clear()                             {if (_root != NULL) delete _root; _root = _first = _last = NULL;}

                /** \return the root node. */
                NodeType        *Root()                             {return _root;}
                /** \return the root node. */
                const NodeType  *Root() const                       {return _root;}
                /** \return the first node in the sorted tree. */
                NodeType        *First()                            {return _first;}
                /** \return the last node in the sorted tree. */
                NodeType        *Last()                             {return _last;}
                /** \return the first node in the sorted tree. */
                const NodeType  *First() const                      {return _first;}
                /** \return the last node in the sorted tree. */
                const NodeType  *Last() const                       {return _last;}

                /** \return true if the tree is empty */
                bool            Empty() const                       {return (_root == NULL);}

                /**
                    Search a key into the tree.
                    \return the node equals to the given key. return NULL if nothing is found.
                */
                NodeType        *Search(const T &key);

                /** Insert the given key into the tree */
                void            Insert(const T &key);

                /**
                    Remove the first data which match with the given \p key.
                    \param data filled with the data of the removed node if not null and if a node has been removed.
                    \return true if a node has been removed.
                */
                bool            Remove(const T &key, T *data = NULL);
                /**
                    Remove the first data in the tree.
                    \param data filled with the data of the removed node if not null and if a node has been removed.
                    \return true if a node has been removed.
                */
                bool            RemoveFirst(T *data = NULL);
                /**
                    Remove the last data in the tree.
                    \param data filled with the data of the removed node if not null and if a node has been removed.
                    \return true if a node has been removed.
                */
                bool            RemoveLast(T *data = NULL);

            protected:
                NodeType        *_root;             ///< The root of the tree.
                NodeType        *_first;            ///< Store the first node on the sorted tree
                NodeType        *_last;             ///< Store the last node on the sorted tree
        };

        template<typename T, template<typename> class N>
        BinaryTree<T,N>::BinaryTree(const BinaryTree &bt)
        {
            if (bt._root != NULL)
            {
                _root = new NodeType(*bt._root);
                NodeType *n;
                for (n = _root; n->Prev() != NULL; n = n->Prev());
                _first = n;
                for (n = _root; n->Next() != NULL; n = n->Next());
                _last = n;
            }
            else
            {
                _root = _first = _last = NULL;
            }
        }

        template<typename T, template<typename> class N>
        BinaryTree<T,N> &BinaryTree<T,N>::operator = (const BinaryTree &bt)
        {
            if (_root != NULL)
                delete _root;
            if (bt._root != NULL)
            {
                _root = new NodeType(*bt._root);
                NodeType *n;
                for (n = _root; n->Prev() != NULL; n = n->Prev());
                _first = n;
                for (n = _root; n->Next() != NULL; n = n->Next());
                _last = n;
            }
            else
            {
                _root = _first = _last = NULL;
            }
            return *this;
        }

        template<typename T, template<typename> class N>
        typename BinaryTree<T,N>::NodeType    *BinaryTree<T,N>::Search(const T &key)
        {
            if (_root != NULL)
            {
                NodeType *n = _root->Search(key);
                return ((n) ? n->data : NULL);
            }
            else
                return NULL;
        }

        template<typename T, template<typename> class N>
        void        BinaryTree<T,N>::Insert(const T &key)
        {
            NodeType *n = new NodeType(key);
            _root = (_root != NULL) ? _root->Insert(n) : n;

            // record first-last node
            if (_first == NULL || n->data < _first->data)
                _first = n;
            if (_last == NULL || _last->data < n->data)
                _last = n;
            while (_first != NULL && _first->Left() != NULL)
                _first = _first->Left();
            while (_last != NULL && _last->Right() != NULL)
                _last = _last->Right();
        }

        template<typename T, template<typename> class N>
        bool        BinaryTree<T,N>::Remove(const T &key, T *data)
        {
            if (_root != NULL)
            {
                // if the firsts nodes are equals, it is possible that the _first is not on the first possition.
                // to we check that possibility. same for _last
                for (; _first->Prev() != NULL; _first = _first->Prev());
                for (; _last->Next() != NULL; _last = _last->Next());

                if (_first != NULL && _first->data == key)
                    _first = _first->Next();
                if (_last != NULL && _last->data == key)
                    _last = _last->Prev();

                NodeType *n = NULL;
                _root = _root->Remove(key, n);
                if (data != NULL && n != NULL)
                    *data = n->data;
                if (n != NULL)
                {
                    delete n;
                    return true;
                }
            }
            return false;
        }

        template<typename T, template<typename> class N>
        bool            BinaryTree<T,N>::RemoveFirst(T *data)
        {
            if (_root != NULL)
            {
                // if the firsts nodes are equals, it is possible that the _first is not on the first possition.
                // to we check that possibility. same for _last
                for (; _first->Prev() != NULL; _first = _first->Prev());
                for (; _last->Next() != NULL; _last = _last->Next());

                NodeType *f = _first;
                *data = _first->data;
                if (_last == _first)
                    _last = _last->Prev();
                _first = _first->Next();

                _root = _root->Remove(f);
                delete f;
                return true;
            }
            return false;
        }

        template<typename T, template<typename> class N>
        bool            BinaryTree<T,N>::RemoveLast(T *data)
        {
            if (_root != NULL)
            {
                // if the firsts nodes are equals, it is possible that the _first is not on the first possition.
                // to we check that possibility. same for _last
                for (; _first->Prev() != NULL; _first = _first->Prev());
                for (; _last->Next() != NULL; _last = _last->Next());

                NodeType *f = _last;
                *data = _last->data;
                if (_last == _first)
                    _first = _first->Next();
                _last = _last->Prev();

                _root = _root->Remove(f);
                delete f;
                return true;
            }
            return false;
        }
    }
}

#endif
