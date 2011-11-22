
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        03/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SPACEPARTITIONING_SUBTREE_H_
#define NC_GRAPHICS_SPACEPARTITIONING_SUBTREE_H_

#include "../Scene/SceneNode.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a Subtree SceneNode for partionning space or partitionning objects data
        /**
            A subtree is defined by a politic so you need to know the reel type to see the comportement and the structure of the subtree

            \sa
                - Octree
        */
        template<template<typename,class,class> class P>
        class SubTree : public AbstractSceneNode<P>
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(ISceneNode);
                typedef AbstractSceneNode<P>        MotherType;

            public:
                SubTree(const char *className)
                    : MotherType(className)             {}
                virtual ~SubTree()                      {}

                /** Clone the scene node */
                virtual ISceneNode      *Clone() const = 0;

                /**
                    Insert the given \p node into the current node at the given position \p at and remode it from the given \p oldParent at the given ]p oldAt.
                    Throw an exception if the type of the given \p node is imcompatible with the current node. (Entity and Subtree are incompatible)
                */
                virtual void                Move(ISceneNode *node, int at, ISceneNode *oldParent, int oldAt)            {throw Utils::Exception("SubTree::Move", "The movement into a subtree is forbiden.");}

            protected:
                /** Update the node with the elapsed time in second */
                virtual void            Update(float elapsedTime) = 0;
                /** Render the node */
                virtual void            Render(SceneGraph *scene) = 0;
        };
    }
}

#endif
