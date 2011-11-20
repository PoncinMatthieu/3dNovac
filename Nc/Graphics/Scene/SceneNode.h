
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
        class LGRAPHICS ISceneNode : public System::Object
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(System::Object);
                NC_UTILS_DEFINE_VISITABLE(System::Object);
                NC_UTILS_DEFINE_INVOKABLE(System::Object);

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
                /**
                    Insert the given \p node into the current node at the given position \p at and remode it from the given \p oldParent at the given ]p oldAt.
                    Throw an exception if the type of the given \p node is imcompatible with the current node. (Entity and Subtree are incompatible)
                */
                virtual void                Move(ISceneNode *node, int at, ISceneNode *oldParent, int oldAt) = 0;

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
                virtual void                 Enable(bool status)                             {_enabled = status;}
                /** Enable the node */
                virtual void                 Enable()                                        {_enabled = true;}
                /** Disable the node */
                virtual void                 Disable()                                       {_enabled = false;}
                /** \return the enable statement */
                virtual bool                 Enabled() const                                 {return _enabled;}

                /** \return false if the object or one of its parents has the enable statement to false */
                bool                         EnabledRecursif() const;

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
                virtual ~AbstractSceneNode()                                {}

                /** \return the number of childs */
                virtual unsigned int        ChildCount() const              {return NodePolitic::ChildCount();}
                /** \return the good child with the given \p index */
                virtual const ISceneNode    *Child(unsigned int i) const    {return NodePolitic::Child(i);}

                friend LGRAPHICS std::ostream& operator << (std::ostream& Out, const AbstractSceneNode<P>& o)
                {
                    Out << static_cast<const ISceneNode&>(o);
                    return Out;
                }
        };
    }
}


#endif
