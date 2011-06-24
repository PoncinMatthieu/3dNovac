
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

#ifndef NC_CORE_TREE_NNODEPOLITIC_H_
#define NC_CORE_TREE_NNODEPOLITIC_H_

#include "../Math/Array.h"
#include "INodePolitic.h"

namespace Nc
{
    namespace Graph
    {
        /// Politic class to define an N node, has an Array of childs, But if N is equal to 0, then the node has a list of childs.
        template<typename T, class NodeType, unsigned int NbChilds = 0, bool Graph = false, class Allocator = Utils::Metaprog::Allocator<NodeType> >
        class NNodePolitic : public INodePolitic<T,NodeType,Graph>
        {
            public:
                typedef INodePolitic<T,NodeType,Graph>          NodePolitic;
                typedef Math::Array<NodeType*, NbChilds>        ContainerType;

            public:
                NNodePolitic()                                      : NodePolitic()                 {_childs.Init(NULL);}
                NNodePolitic(const T &data)                         : NodePolitic(data)             {_childs.Init(NULL);}
                NNodePolitic(NodeType *parent)                      : NodePolitic(parent)           {_childs.Init(NULL);}
                NNodePolitic(const T &data, NodeType *parent)       : NodePolitic(data, parent)     {_childs.Init(NULL);}
                NNodePolitic(const NNodePolitic &nn);
                NNodePolitic &operator = (const NNodePolitic &nn);
                ~NNodePolitic();

                /** \return the childs of the node */
                ContainerType       &Childs()                           {return _childs;}
                /** \return the childs of the node */
                const ContainerType &Childs() const                     {return _childs;}

                /** \return the height of the node (length of the longest downward path to a leaf) */
                int                 Height() const;

                /** \return true if the node has at least one child */
                bool                HasChilds() const                   {return (NbChilds != 0);}
                /** \return the number of child nodes */
                unsigned int        ChildCount() const                  {return (NbChilds);}
                /** \return the ieme child */
                NodeType            *Child(unsigned int i)              {return _childs[i];}
                /** \return the ieme child */
                const NodeType      *Child(unsigned int i) const        {return _childs[i];}

                /** Remove the childs of the node */
                void                RemoveChilds();
                /**
                    Remove the given child and dealocate it with the allocator.
                    \return true if the node has been found and removed
                */
                bool                RemoveChild(NodeType *n);

                /** Extract all data's leafs of the node into the given list */
                void                ExtractLeaf(std::list<T> &leafs) const;
                /** Extract all data's childs of the node into the given list */
                void                ExtractChilds(std::list<T> &childs) const;

            protected:
                ContainerType       _childs;            ///< the childs Array of the node
                Allocator           _alloc;             ///< the allocator use to allocate and construct the childs node
        };

        /// Specialization for a NNodePolitic witch contain a list of childs
        template<typename T, class NodeType, bool Graph, class Allocator>
        class NNodePolitic<T,NodeType,0,Graph,Allocator> : public INodePolitic<T,NodeType,Graph>
        {
            public:
                typedef INodePolitic<T,NodeType,Graph>      NodePolitic;
                typedef std::list<NodeType*>                ContainerType;

            public:
                NNodePolitic()                                      : NodePolitic()                 {}
                NNodePolitic(const T &data)                         : NodePolitic(data)             {}
                NNodePolitic(NodeType *parent)                      : NodePolitic(parent)           {}
                NNodePolitic(const T &data, NodeType *parent)       : NodePolitic(data, parent)     {}
                NNodePolitic(const NNodePolitic &nn);
                NNodePolitic &operator = (const NNodePolitic &nn);
                ~NNodePolitic();

                /** \return the childs of the node */
                ContainerType       &Childs()                           {return _childs;}
                /** \return the childs of the node */
                const ContainerType &Childs() const                     {return _childs;}

                /** \return the height of the node (length of the longest downward path to a leaf) */
                int                 Height() const;

                /** \return true if the node has at least one child */
                bool                HasChilds() const                   {return (!_childs.empty());}
                /** \return the number of child nodes */
                unsigned int        ChildCount() const                  {return (_childs.size());}
                /** \return the ieme child */
                NodeType            *Child(unsigned int i);
                /** \return the ieme child */
                const NodeType      *Child(unsigned int i) const;

                /**
                    Add a child to the node.
                    Automaticaly set the parent of the child.
                */
                void                AddChild(NodeType *child)           {child->SetParent(static_cast<NodeType*>(this)); _childs.push_back(child);}

                /** Remove the childs of the node */
                void                RemoveChilds();
                /**
                    Remove the given child and dealocate it with the allocator.
                    \return true if the node has been found and removed
                */
                bool                RemoveChild(NodeType *n);

                /** Extract all data's leafs of the node into the given list */
                void                ExtractLeaf(std::list<T> &leafs) const;
                /** Extract all data's childs of the node into the given list */
                void                ExtractChilds(std::list<T> &childs) const;

            protected:
                ContainerType       _childs;        ///< the child list of the node
                Allocator           _alloc;         ///< the allocator used to allocate and construct the childs node
        };

        /// Politic to used to define a node with a list of childs
        template<typename T, class NodeType, class Allocator>
        struct ListNodePolitic : public NNodePolitic<T,NodeType,0,true, Allocator>
        {
            typedef NNodePolitic<T,NodeType,0,true,Allocator>   NodePolitic;

            ListNodePolitic()                                   : NodePolitic()                 {}
            ListNodePolitic(const T &data)                      : NodePolitic(data)             {}
            ListNodePolitic(NodeType *parent)                   : NodePolitic(parent)           {}
            ListNodePolitic(const T &data, NodeType *parent)    : NodePolitic(data, parent)     {}
            ~ListNodePolitic()                                  {}
        };

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::NNodePolitic(const NNodePolitic &nn)
            : NodePolitic(nn)
        {
            for (unsigned int i = 0; i < NbChilds; ++i)
            {
                if (nn._childs.Data[i] != NULL)
                {
                    _childs.Data[i] = _alloc.Allocate(*nn._childs.Data[i]);
                    _childs.Data[i]->SetParent(static_cast<NodeType*>(this));
                }
                else
                    _childs.Data[i] = NULL;
            }
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,NbChilds,Graph,Allocator> &NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::operator = (const NNodePolitic &nn)
        {
            for (unsigned int i = 0; i < NbChilds; ++i)
            {
                if (_childs.Data[i] != NULL)
                {
                    _childs.Data[i]->UnParent(static_cast<NodeType*>(this));
                    if (_childs.Data[i]->IsRoot())
                        _alloc.Deallocate(_childs.Data[i]);
                }
                if (nn._childs.Data[i] != NULL)
                {
                    _childs.Data[i] = _alloc.Allocate(*nn._childs.Data[i]);
                    _childs.Data[i]->SetParent(static_cast<NodeType*>(this));
                }
                else
                    _childs.Data[i] = NULL;
            }
            return *this;
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,0,Graph,Allocator>::NNodePolitic(const NNodePolitic &nn)
            : NodePolitic(nn)
        {
            for (typename ContainerType::const_iterator it = nn._childs.begin(); it != nn._childs.end(); ++it)
            {
                if (*it != NULL)
                {
                    NodeType *newChild = _alloc.Allocate(**it);
                    newChild->SetParent(static_cast<NodeType*>(this));
                    _childs.push_back(newChild);
                }
            }
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,0,Graph,Allocator> &NNodePolitic<T,NodeType,0,Graph,Allocator>::operator = (const NNodePolitic &nn)
        {
            // free
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end(); it = _childs.erase(it))
                if (*it != NULL)
                {
                    (*it)->UnParent(static_cast<NodeType*>(this));
                    if ((*it)->IsRoot())
                        _alloc.Deallocate(*it);
                }
            // copy
            for (typename ContainerType::const_iterator it = nn._childs.begin(); it != nn._childs.end(); ++it)
            {
                if (*it != NULL)
                {
                    NodeType *newChild = _alloc.Allocate(**it);
                    newChild->SetParent(static_cast<NodeType*>(this));
                    _childs.push_back(newChild);
                }
            }
            return *this;
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::~NNodePolitic()
        {
            for (unsigned int i = 0; i < NbChilds; ++i)
            {
                if (_childs.Data[i] != NULL)
                {
                    _childs.Data[i]->UnParent(static_cast<NodeType*>(this));
                    if (_childs.Data[i]->IsRoot())
                        _alloc.Deallocate(_childs.Data[i]);
                }
            }
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        NNodePolitic<T,NodeType,0,Graph,Allocator>::~NNodePolitic()
        {
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
            {
                (*it)->UnParent(static_cast<NodeType*>(this));
                if ((*it)->IsRoot())
                    _alloc.Deallocate(*it);
            }
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        NodeType    *NNodePolitic<T,NodeType,0,Graph,Allocator>::Child(unsigned int index)
        {
            if (index < _childs.size())
            {
                typename ContainerType::iterator it = _childs.begin();
                for (unsigned int i = 0; i < index; ++it, ++i);
                    return (*it);
            }
            return NULL;
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        const NodeType  *NNodePolitic<T,NodeType,0,Graph,Allocator>::Child(unsigned int index) const
        {
            if (index < _childs.size())
            {
                typename ContainerType::const_iterator it = _childs.begin();
                for (unsigned int i = 0; i < index; ++it, ++i);
                    return (*it);
            }
            return NULL;
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        void    NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::RemoveChilds()
        {
            for (unsigned int i = 0; i < NbChilds; ++i)
            {
                if (_childs.Data[i] != NULL)
                {
                    _childs.Data[i]->UnParent(static_cast<NodeType*>(this));
                    if (_childs.Data[i]->IsRoot())
                        _alloc.Deallocate(_childs.Data[i]);
                    _childs.Data[i] = NULL;
                }
            }
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        void    NNodePolitic<T,NodeType,0,Graph,Allocator>::RemoveChilds()
        {
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end();)
            {
                if ((*it) != NULL)
                {
                    (*it)->UnParent(static_cast<NodeType*>(this));
                    if ((*it)->IsRoot())
                        _alloc.Deallocate(*it);
                }
                it = _childs.erase(it);
            }
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        bool    NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::RemoveChild(NodeType *n)
        {
            if (n != NULL)
            {
                unsigned int i = 0;
                for (; i < NbChilds; ++i)
                {
                    if (_childs.Data[i] == n)
                    {
                        _childs.Data[i]->UnParent(static_cast<NodeType*>(this));
                        if (_childs.Data[i]->IsRoot())
                            _alloc.Deallocate(_childs.Data[i]);
                        _childs.Data[i] = NULL;
                        return true;
                    }
                }
                for (i = 0; i < NbChilds; ++i)
                {
                    if (_childs.Data[i] != NULL)
                        if (_childs.Data[i]->RemoveChild(n))
                            return true;
                }
            }
            return false;
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        bool    NNodePolitic<T,NodeType,0,Graph,Allocator>::RemoveChild(NodeType *n)
        {
            if (n != NULL)
            {
                typename ContainerType::iterator it = _childs.begin();
                for (; it != _childs.end(); ++it)
                {
                    if ((*it) == n)
                    {
                        (*it)->UnParent(static_cast<NodeType*>(this));
                        if ((*it)->IsRoot())
                            _alloc.Deallocate(*it);
                        _childs.erase(it);
                        return true;
                    }
                }
                for (it = _childs.begin(); it != _childs.end(); ++it)
                {
                    if ((*it)->RemoveChild(n))
                        return true;
                }
            }
            return false;
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        int     NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::Height() const
        {
            int     height = 0;
            int     heightTmp;

            // get back all child height
            for (unsigned int i = 0; i < NbChilds; ++i)
            {
                if (_childs.Data[i] != NULL)
                {
                    heightTmp = 1 + _childs.Data[i]->Height();
                    if (height < heightTmp)
                        height = heightTmp;
                }
            }
            return height;
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        int         NNodePolitic<T,NodeType,0,Graph,Allocator>::Height() const
        {
            int     height = 0;
            int     heightTmp;

            // get back all child height
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
            {
                heightTmp = 1 + (*it)->Height();
                if (height < heightTmp)
                    height = heightTmp;
            }
            return height;
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        void        NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::ExtractLeaf(std::list<T> &leafs) const
        {
            if (!HasChilds())
                leafs.push_back(NodePolitic::Data);
            for (unsigned int i = 0; i < NbChilds; ++i)
                if (_childs.Data[i] != NULL)
                _childs.Data[i]->ExtractLeaf(leafs);
        }

        template<typename T, class NodeType, unsigned int NbChilds, bool Graph, class Allocator>
        void        NNodePolitic<T,NodeType,NbChilds,Graph,Allocator>::ExtractChilds(std::list<T> &childs) const
        {
            childs.push_back(NodePolitic::Data);
            for (unsigned int i = 0; i < NbChilds; ++i)
                if (_childs.Data[i] != NULL)
                    _childs.Data[i]->ExtractChilds(childs);
        }

        template<typename T, class NodeType, bool Graph, class Allocator>
        void        NNodePolitic<T,NodeType,0,Graph,Allocator>::ExtractLeaf(std::list<T> &leafs) const
        {
            if (!HasChilds())
                leafs.push_back(NodePolitic::Data);
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
                (*it)->ExtractLeaf(leafs);
        }
        template<typename T, class NodeType, bool Graph, class Allocator>
        void        NNodePolitic<T,NodeType,0,Graph,Allocator>::ExtractChilds(std::list<T> &childs) const
        {
            childs.push_back(NodePolitic::Data);
            for (typename ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
                (*it)->ExtractChilds(childs);
        }

    }
}

#endif
