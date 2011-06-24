
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

    File Created At:        23/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_SCENENODE_H_
#define NC_GRAPHICS_SCENE_SCENENODE_H_

#include <Nc/Core/Graph/DotGraph.h>
#include <Nc/Core/Graph/NNodePolitic.h>
#include <Nc/Core/Graph/Node.h>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a scene node
        class ISceneNode : public System::Object
        {
            public:
                struct Allocator
                {
                    template<class NodeType>
                    NodeType            *Allocate(const NodeType &toCopy)       {return static_cast<NodeType*>(toCopy.Clone());}
                    inline void         Deallocate(ISceneNode *p)               {delete p;}
                };

            public:
                ISceneNode(const char *className)   : System::Object(className), _enabled(true)     {}
                virtual ~ISceneNode()               {}

                /**
                    \return a copy of the node
                    This method should be redefined in all inherited child
                */
                virtual ISceneNode          *Clone() const = 0;

                /** Update the node and it's childs */
                virtual void                Update(float elapsedTime) = 0;

                /** Render the node and it's childs */
                virtual void                Render(SceneGraph *scene) = 0;

                /** \return the number of childs */
                virtual unsigned int        ChildCount() const = 0;
                /** \return the good child with the given \p index */
                virtual const ISceneNode    *Child(unsigned int i) const = 0;

                /** \return the number of subtree */
                virtual unsigned int        SubTreeCount() const = 0;
                /** \return the good subtree with the given \p index */
                virtual const ISceneNode    *SubTree(unsigned int i) const = 0;

                /** \return the type of the string */
                std::string                 NodeTypeToString() const                        {return GetClassName();}
                /** \return Save the scene node hierarchie in a dot file */
                void                        SaveDot(const Utils::FileName &file) const      {Graph::DotGraph<ISceneNode,true>::Save(file, *this);}

                /** Lock the node, to use for exemple when you want to remove a child, this will protect the node with a mutex for a multithreaded context */
                inline void                 Lock()                                          {_mutex.Lock();}
                /** Unlock the node */
                inline void                 Unlock()                                        {_mutex.Unlock();}

                /** Set the enable statement of the node */
                inline void                 Enable(bool status)                             {_enabled = status;}
                /** Enable the node */
                inline void                 Enable()                                        {_enabled = true;}
                /** Disable the node */
                inline void                 Disable()                                       {_enabled = false;}
                /** \return the enable statement */
                inline bool                 Enabled() const                                 {return _enabled;}

            protected:
                System::Mutex       _mutex;         ///< a mutex used to protect the node, (this mutex is lock at the render pass)
                bool                _enabled;       ///< an enable statement
        };

        /// Define an abstract scene node
        template<template<typename,class,class> class P>
        class AbstractSceneNode : public ISceneNode, public Graph::AbstractNode<ISceneNode*, P, AbstractSceneNode<P>, ISceneNode::Allocator>
        {
            public:
                typedef AbstractSceneNode<P>                                                    NodeType;
                typedef Graph::AbstractNode<ISceneNode*, P, NodeType, ISceneNode::Allocator>    NodePolitic;

                using ISceneNode::NodeTypeToString;

            public:
                AbstractSceneNode(const char *className)                    : ISceneNode(className), NodePolitic()              {}
                AbstractSceneNode(const char *className, ISceneNode *data)  : ISceneNode(className), NodePolitic(data)          {}
                virtual ~AbstractSceneNode()                                {}

                /** \return the number of childs */
                virtual unsigned int        ChildCount() const              {return NodePolitic::ChildCount();}
                /** \return the good child with the given \p index */
                virtual const ISceneNode    *Child(unsigned int i) const    {return NodePolitic::Child(i);}

                /** Recursivelly call a fonctor for itself and it's childs using a postfix or a suffixe traversal */
                template<bool Postfix, typename Fonctor>
                bool        ForEachChilds(Fonctor &f);
                /** Recursivelly call a fonctor for itself and it's childs using a postfix or a suffixe traversal */
                template<bool Postfix, typename Fonctor>
                bool        ForEachChilds(Fonctor &f) const;

                /** Recursivelly call a fonctor for itself and it's parents using a postfix or a suffixe traversal */
                template<bool Postfix, typename Fonctor>
                bool        ForEachParents(Fonctor &f);
                /** Recursivelly call a fonctor for itself and it's parents using a postfix or a suffixe traversal */
                template<bool Postfix, typename Fonctor>
                bool        ForEachParents(Fonctor &f) const;

                friend std::ostream& operator << (std::ostream& Out, const AbstractSceneNode<P>& o)
                {
                    Out << static_cast<const ISceneNode&>(o);
                    return Out;
                }
        };

        /// Define an entity node
        /**
            An entity is define by a Matrix and can contain only one subtree and a list of childs
        */
        class Entity : public AbstractSceneNode<Graph::ListNodePolitic>
        {
            public:
                typedef AbstractSceneNode<Graph::ListNodePolitic>            NodePolitic;

            public:
                Entity()                                        : NodePolitic(ClassName())      {}
                Entity(const char *className)                   : NodePolitic(className)        {}
                Entity(const char *className, const TMatrix &m) : NodePolitic(className), Matrix(m) {}
                Entity(const char *className, ISceneNode *data) : NodePolitic(className, data)  {}
                Entity(const Entity &n)                         : NodePolitic(n)                {if (NodePolitic::Data != NULL) NodePolitic::Data = n.Data->Clone();}
                Entity &operator = (const Entity &n)                                            {if (NodePolitic::Data != NULL) NodePolitic::Data = n.Data->Clone(); return *this;}
                virtual ~Entity()                                                               {if (Data != NULL) delete Data;}

                static const char           *ClassName()                                        {return "Entity";}
                virtual ISceneNode          *Clone() const                                      {return new Entity(*this);}

                /** Render the node */
                virtual void                Render(SceneGraph *scene)                           {_mutex.Lock(); RenderChilds(scene); _mutex.Unlock();}
                /** Update the node */
                virtual void                Update(float elapsedTime)                           {UpdateChilds(elapsedTime);}

                /** \return the number of subtree witch can contain a least one subtree */
                virtual unsigned int        SubTreeCount() const                                {return ((Data == NULL) ? 0 : 1);}
                /** \return the subtree */
                virtual const ISceneNode    *SubTree(unsigned int) const                        {return Data;}

                /** Render the childs and the subtrees */
                void                        RenderChilds(SceneGraph *scene);
                /** Update the childs and the subtrees */
                void                        UpdateChilds(float elapsedTime);

            public:
                TMatrix                     Matrix;                 ///< the matrix of the entity
        };

        template<template<typename,class,class> class P>
        template<bool Postfix, typename Fonctor>
        bool    AbstractSceneNode<P>::ForEachChilds(Fonctor &f)
        {
            /// \todo find a way to go through the subtree
            //if (NodeType::Data != NULL)
            //    static_cast<Graph::Container<ISceneNode*>*>(this->Data)->ForEachChilds(f);

            typename NodePolitic::ContainerType &childs = NodePolitic::Childs();
            bool                                state = true;
            if (Postfix)
            {
                for (typename NodePolitic::ContainerType::iterator it = childs.begin(); it != childs.end(); ++it)
                    if (!(*it)->ForEachChilds<Postfix>(f))
                        state = false;
                if (!state)
                    return false;
                state = f(this);
            }
            else
            {
                if (!f(this))
                    return false;
                for (typename NodePolitic::ContainerType::iterator it = childs.begin(); it != childs.end(); ++it)
                    if (!(*it)->ForEachChilds<Postfix>(f))
                        state = false;
            }
            return state;
        }

        template<template<typename,class,class> class P>
        template<bool Postfix, typename Fonctor>
        bool    AbstractSceneNode<P>::ForEachChilds(Fonctor &f) const
        {
            /// \todo find a way to go through the subtree
            //if (NodeType::Data != NULL)
            //    static_cast<Graph::Container<ISceneNode*>*>(this->Data)->ForEachChilds(f);

            typename NodePolitic::ContainerType &childs = NodePolitic::Childs();
            bool                                state = true;
            if (Postfix)
            {
                for (typename NodePolitic::ContainerType::const_iterator it = childs.begin(); it != childs.end(); ++it)
                    if (!(*it)->ForEachChilds<Postfix>(f))
                        state = false;
                if (!state)
                    return false;
                state = f(this);
            }
            else
            {
                if (!f(this))
                    return false;
                for (typename NodePolitic::ContainerType::const_iterator it = childs.begin(); it != childs.end(); ++it)
                    if (!(*it)->ForEachChilds<Postfix>(f))
                        state = false;
            }
            return state;
        }

        template<template<typename,class,class> class P>
        template<bool Postfix, typename Fonctor>
        bool    AbstractSceneNode<P>::ForEachParents(Fonctor &f)
        {
            /// \todo find a way to go through the subtree
            //if (NodeType::Data != NULL)
            //    NodeType::Data->ForEachParents(f);

            if (Postfix)
            {
                for (typename NodeType::ParentList::iterator it = NodeType::_parents.begin(); it != NodeType::_parents.end(); ++it)
                    if (!(*it)->ForEachParents<Postfix>(f))
                        return false;
                if (!f(this))
                    return false;
            }
            else
            {
                if (!f(this))
                    return false;
                for (typename NodeType::ParentList::iterator it = NodeType::_parents.begin(); it != NodeType::_parents.end(); ++it)
                    if (!(*it)->ForEachParents<Postfix>(f))
                        return false;
            }
            return true;
        }

        template<template<typename,class,class> class P>
        template<bool Postfix, typename Fonctor>
        bool    AbstractSceneNode<P>::ForEachParents(Fonctor &f) const
        {
            /// \todo find a way to go through the subtree
            //if (NodeType::Data != NULL)
            //    NodeType::Data->ForEachParents(f);

            if (Postfix)
            {
                for (typename NodeType::ParentList::const_iterator it = NodeType::_parents.begin(); it != NodeType::_parents.end(); ++it)
                    if (!(*it)->ForEachParents<Postfix>(f))
                        return false;
                if (!f(this))
                    return false;
            }
            else
            {
                if (!f(this))
                    return false;
                for (typename NodeType::ParentList::const_iterator it = NodeType::_parents.begin(); it != NodeType::_parents.end(); ++it)
                    if (!(*it)->ForEachParents<Postfix>(f))
                        return false;
            }
            return true;
        }
    }
}


#endif
