
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

#ifndef NC_GRAPHICS_SPACEPARTITIONING_OCTREE_H_
#define NC_GRAPHICS_SPACEPARTITIONING_OCTREE_H_

#include <Nc/Core/Graph/Octree.h>
#include "SubTree.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define an octree used to store nodes and apply some rendering / tree search like Frustum culling
        /**
            An octree can have 8 childs and has a list of ISceneNode as subtrees
        */
        class Octree : public SubTree<Graph::OctreeNodePolitic>
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(ISceneNode);
                NC_UTILS_DEFINE_VISITABLE(System::Object);
                NC_UTILS_DEFINE_INVOKABLE(System::Object);

                /** The pattern define the comportement of the octree */
                enum Pattern
                {
                    Nop,                ///< Nothing special
                    FrustumCulling      ///< The octree node will be rendered with frustum culling by using the current camera frustum of the scene
                };

            public:
                Octree(const Vector3f &center, unsigned int size);
                Octree(const Box3f &box);
                Octree(const Octree &oct);
                Octree &operator = (const Octree &oct);
                ~Octree()                                                   {Utils::DeleteContainer(Data);}

                static const char       *ClassName()                        {return "Octree";}
                virtual ISceneNode      *Clone() const                      {return new Octree(*this);}

                /** Update all children and subtree */
                virtual void            Update(float elapsedTime)           {UpdateChilds(elapsedTime);}

                /** Render childrens and subtree by using the patten */
                virtual void            Render(SceneGraph *scene)           {_mutex.Lock(); RenderChilds(scene); _mutex.Unlock();}

                /** \return the number of subtree (ISceneNode) into the node */
                virtual unsigned int    SubTreeCount() const                {return Data.size();}
                /** \return the subtree corresponding to the given index */
                virtual const ISceneNode *SubTree(unsigned int i) const;

                /** Render the childs and subtree by using the pattern */
                void                    RenderChilds(SceneGraph *scene);

                /** Update all children and subtree */
                void                    UpdateChilds(float elapsedTime);

                /** Set the draw lines statement (if true, then the outlines lines of the nodes would be displayed) */
                void                    DrawOutlines(bool state);
                /** \return the draw lines statement */
                inline bool             DrawOutlines() const                {return _drawOutlines;}

                /** Insert a list of nodes with their calculated box into the octree (create subnode if needed function by the depthMax) */
                void                    Insert(const std::list<std::pair<Math::Box3f,ISceneNode*> > &datasToInsert, unsigned int depthMax);

                /** Set the pattern of the octree node */
                void                    SetPattern(Pattern p)               {_pattern = p;}

            private:
                bool                    _drawOutlines;          ///< draw outlines statement
                Pattern                 _pattern;               ///< the pattern of the node

                static BoundingBox      *_boundingBoxObject;    ///< a static box to draw the outlines of the octrees
        };
    }
}

#endif
