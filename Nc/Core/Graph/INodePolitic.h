
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

#ifndef NC_CORE_TREE_INODEPOLITIC_H_
#define NC_CORE_TREE_INODEPOLITIC_H_

#include <list>
#include <sstream>
#include <typeinfo>
#include "../Define.h"
#include "../Utils/Logger.h"
#include "../Utils/Exception.h"
#include "../Utils/Metaprog.h"
#include "../Utils/VisitorManager.h"

namespace Nc
{
    namespace Graph
    {
        /// a set of class witch should'nt be used outside of the graph namespace
        namespace Priv
        {
            /// Base class for each node of the graph namespace
            struct INodeBasePolitic : public Utils::VisitableBase<INodeBasePolitic>
            {
                NC_UTILS_DEFINE_PARENT_CLASS(Utils::VisitableBase<INodeBasePolitic>);
                NC_UTILS_DEFINE_VISITABLE(INodeBasePolitic);
                NC_UTILS_DEFINE_INVOKABLE(INodeBasePolitic);
            };

            /// A base class Container to store the data of a node if needed
            template<typename T>
            struct Container
            {
                Container()                                     {}
                Container(const T &data)     : Data(data)       {}

                T       Data;
            };

            /// A base class Container to store the data of a node if needed (specialization for a pointer)
            template<typename T>
            struct Container<T*>
            {
                Container()             : Data(NULL)       {}
                Container(T *data)      : Data(data)       {}

                T       *Data;
            };

            /// Interface used to define a tree node (with a unique parent)
            template<typename NodeType>
            class AbstractTreePolitic
            {
                public:
                    AbstractTreePolitic()                                           : _parent(NULL)     {}
                    AbstractTreePolitic(NodeType *parent)                           : _parent(parent)   {}
                    AbstractTreePolitic(const AbstractTreePolitic &)                : _parent(NULL)     {}
                    AbstractTreePolitic &operator = (const AbstractTreePolitic &)    {_parent = NULL; return (*this);}

                    /** \return true if the node is root (has no parent) */
                    bool            IsRoot() const                                  {return (_parent == NULL);}
                    /** \return the parent of the node (if there is no parent, return NULL) */
                    NodeType        *Parent()                                       {return _parent;}
                    /** \return the parent of the node (if there is no parent, return NULL) */
                    const NodeType  *Parent() const                                 {return _parent;}

                    /** Set the parent of the tree node */
                    void            SetParent(NodeType *parent)                     {_parent = parent;}

                    /** Set the parent to null */
                    void            UnParent(NodeType *)                            {_parent = NULL;}

                    /** \return the root node of the node */
                    NodeType        *Root()
                    {
                        NodeType *root = static_cast<NodeType*>(this);
                        while (root->_parent != NULL)
                            root = root->_parent;
                        return root;
                    }

                    /** \return the root node of the node */
                    const NodeType  *Root() const
                    {
                        NodeType *root = static_cast<NodeType*>(this);
                        while (root->_parent != NULL)
                            root = root->_parent;
                        return root;
                    }

                    /** \return the depth of the node (length of the path to it's root) */
                    int             Depth() const
                    {
                        int n = 0;
                        for (NodeType *root = static_cast<NodeType*>(this); root->Parent() != NULL; ++n)
                            root = root->Parent();
                        return n;
                    }

                protected:
                    NodeType                *_parent;   ///< the parent of the node
            };

            /// Interface used to define a graph node (with a list of parents)
            template<typename NodeType>
            class AbstractGraphPolitic
            {
                public:
                    typedef std::list<NodeType*>    ParentList;

                public:
                    AbstractGraphPolitic()                                          {}
                    AbstractGraphPolitic(NodeType *parent)                          {_parents.push_back(parent);}
                    AbstractGraphPolitic(const AbstractGraphPolitic &)              {}
                    AbstractGraphPolitic operator = (const AbstractGraphPolitic &)  {return (*this);}

                    /** \return true if the node is root (has no parent) */
                    bool                        IsRoot() const                      {return _parents.empty();}
                    /** \return the parent of the node (if there is no parent, return NULL) */
                    std::list<NodeType*>        &Parents()                          {return _parents;}
                    /** \return the parent of the node (if there is no parent, return NULL) */
                    const std::list<NodeType*>  &Parents() const                    {return _parents;}

                    /** Add a parent into the list of parents of the graph node */
                    void                        SetParent(NodeType *parent)         {_parents.push_back(parent);}

                    /** Remove the given parent to the parent list of the graph node */
                    void                        UnParent(NodeType *parent)
                    {
                        for (typename ParentList::iterator it = _parents.begin(); it != _parents.end(); ++it)
                        {
                            if (*it == parent)
                            {
                                _parents.erase(it);
                                return;
                            }
                        }
                    }

                protected:
                    ParentList      _parents;       ///< the list of parents of the node
            };
        }

        /// Interface to define a node politic
        template<typename T, class NodeType, bool Graph>
        class INodePolitic :    public Priv::INodeBasePolitic, public Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >,
                                public Priv::Container<T>
        {
            public:
                INodePolitic()                                  : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(), Priv::Container<T>()            {}
                INodePolitic(const T &data)                     : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(), Priv::Container<T>(data)        {}
                INodePolitic(NodeType *parent)                  : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(), Priv::Container<T>()			{SetParent(parent);}
                INodePolitic(const T &data, NodeType *parent)   : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(), Priv::Container<T>(data)		{SetParent(parent);}

                /** \return the string of the contained type */
                std::string         TypeToString() const                        {return typeid(T).name();}
                /** \return the string of the node politic type */
                std::string         NodeTypeToString() const                    {return typeid(NodeType).name();}

                /** Print the data of the node to the stream */
                friend LCORE std::ostream     &operator << (std::ostream &os, const INodePolitic &n)
                {
                    os << n.Data;
                    return os;
                }
        };

        /// Specialization of INodePolitic for a node without data
        template<class NodeType, bool Graph>
        class INodePolitic<Utils::Metaprog::Nop, NodeType, Graph> : public Priv::INodeBasePolitic, public Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >
        {
            public:
                INodePolitic()                                                  : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >()          {}
                INodePolitic(const Utils::Metaprog::Nop &)                      : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >()          {}
                INodePolitic(NodeType *parent)                                  : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(parent)    {}
                INodePolitic(const Utils::Metaprog::Nop &, NodeType *parent)    : Utils::Metaprog::If<Graph, Priv::AbstractGraphPolitic<NodeType>, Priv::AbstractTreePolitic<NodeType> >(parent)    {}

                /** \return the string of the contained type */
                std::string         TypeToString() const                        {return typeid(Utils::Metaprog::Nop).name();}
                /** \return the string of the node politic type */
                std::string         NodeTypeToString() const                    {return typeid(NodeType).name();}

                /** Print the data of the node to the stream */
                friend LCORE std::ostream     &operator << (std::ostream &oss, const INodePolitic &)
                {
                    return oss;
                }
        };

        /// Define a visiting method
        enum VisitMethod
        {
            VisitParents,
            VisitChilds
        };

        /// Base class to visit a node of the graph namespace
        /**
            To use this visitor, you have to implement the VisitNode method and the InvokeChild method in your own visitors.
        */
        template<typename VisitorType, typename ReturnType = void, typename Base = Priv::INodeBasePolitic>
        class AbstractNodeVisitor : public Utils::VisitorManager::InvokableVisitor<VisitorType, Base, ReturnType>
        {
            public:
                NC_UTILS_DEFINE_VISIT_INVOKER(NodeInvoker, InvokeNode);
                NC_UTILS_DEFINE_VISIT_INVOKER(NodeVisiter, VisitNode);

            public:
                template<typename ToVisitList, typename VisitableList>
                AbstractNodeVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, VisitMethod visitMethod = VisitChilds, bool postVisits = true)
                    :   Utils::VisitorManager::InvokableVisitor<VisitorType, Base, ReturnType>(postVisits),
                        _visitMethod(visitMethod)
                {
                    InitVTableInvokable(visitableList, NodeInvoker());
                    InitVTable(toVisitList, NodeVisiter());
                }

                template<class NodeType>
                void InvokeNode(NodeType &n)
                {
                    switch (_visitMethod)
                    {
                        case VisitParents:        static_cast<VisitorType*>(this)->InvokeParents(n);   break;
                        case VisitChilds:         static_cast<VisitorType*>(this)->InvokeChilds(n);    break;
                    }
                }

                template<class NodeType>
                void InvokeNode(const NodeType &n)
                {
                    switch (_visitMethod)
                    {
                        case VisitParents:        static_cast<VisitorType*>(this)->InvokeParents(n);   break;
                        case VisitChilds:         static_cast<VisitorType*>(this)->InvokeChilds(n);    break;
                    }
                }

                template<class NodeType>
                void InvokeParents(Priv::AbstractGraphPolitic<NodeType> &node)
                {
                    for (typename Priv::AbstractGraphPolitic<NodeType>::ParentList::iterator it = node.Parents().begin(); it != node.Parents().end(); ++it)
                        if (*it != NULL)
                            (*this)(**it);
                }

                template<class NodeType>
                void InvokeParents(const Priv::AbstractGraphPolitic<NodeType> &node)
                {
                    for (typename Priv::AbstractGraphPolitic<NodeType>::ParentList::const_iterator it = node.Parents().begin(); it != node.Parents().end(); ++it)
                        if (*it != NULL)
                            (*this)(**it);
                }

                template<class NodeType>
                void InvokeParents(Priv::AbstractTreePolitic<NodeType> &node)
                {
                    if (node.Parent() != NULL)
                        (*this)(*node.Parent());
                }

                template<class NodeType>
                void InvokeParents(const Priv::AbstractTreePolitic<NodeType> &node)
                {
                    if (node.Parent() != NULL)
                        (*this)(*node.Parent());
                }

            protected:
                VisitMethod     _visitMethod;
        };
    }
}

#endif
