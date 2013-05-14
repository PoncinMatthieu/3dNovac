
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

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

    File Created At:        16/10/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_ENTITY_H_
#define NC_GRAPHICS_SCENE_ENTITY_H_

#include "SceneNode.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define an entity node.
        /**
            An entity is define by a Matrix and can contain only a subtree and a list of childs.
        */
        class LIB_NC_GRAPHICS Entity : public AbstractSceneNode<Graph::ListNodePolitic>
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(ISceneNode, System::Object, System::Object, Nc::Graphic::Entity);
                typedef AbstractSceneNode<Graph::ListNodePolitic>            NodePolitic;

            public:
                Entity();
                Entity(const TMatrix &m);
                Entity(const Box3f &box);
                Entity(const Box3f &box, const TMatrix &m);
                Entity(const Entity &n);
                Entity &operator = (const Entity &n);
                virtual ~Entity();

                virtual ISceneNode          *Clone() const                                      {return new Entity(*this);}

                /** \return the number of subtree which can contain a least one subtree. */
                virtual unsigned int        SubTreeCount() const                                {return ((data == NULL) ? 0 : 1);}
                /** \return the subtree. */
                virtual const ISceneNode    *SubTree(unsigned int) const                        {return data;}

                /**
                    Insert the given \p node into the current node at the given position \p at and remode it from the given \p oldParent at the given \p oldAt.

                    Throw an exception if the type of the given \p node is imcompatible with the current node. (Entity and Subtree are incompatible).
                */
                virtual void                MoveNode(ISceneNode *node, int at, ISceneNode *oldParent, int oldAt);

                /** Render the childs and the subtrees. */
                void                        RenderChilds(SceneGraph *scene);
                /** Update the childs and the subtrees. */
                void                        UpdateChilds(float elapsedTime);

                /** Set the box. */
                inline void         SetBox(const Box3f &box)            {_box = box;}
                /** \return the box (the box is not modified by the matrix, so if you want the reel box call the method GetReelBox). */
                inline const Box3f  &GetBox() const                     {return _box;}
                /** \param box filled by the box transformed by the model matrix. */
                void                GetReelBox(Box3f &box) const;

                // operations sur la matrice
                /** Scale the object with the given height. */
                void                HeightScale(float height); // /!\ metre a jour la bounding box avant
                /** Translate the model Matrix to the given vector using the xy center of the box. */
                void                CenterBase(const Vector3f &centerBase);

            protected:
                /** Render the node. */
                virtual void                Render(SceneGraph *scene);
                /** Update the node. */
                virtual void                Update(float elapsedTime);

            public:
                TMatrix                     Matrix;                 ///< the matrix of the entity.

            protected:
                Box3f                       _box;                   ///< the box of the entity.
        };
    }
}


#endif

