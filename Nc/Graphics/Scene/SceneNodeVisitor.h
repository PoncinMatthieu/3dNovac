
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

    File Created At:        13/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_SCENENODEVISITOR_H_
#define NC_GRAPHICS_SCENE_SCENENODEVISITOR_H_

#include "SceneNode.h"
#include "../SpacePartitioning/Octree.h"

namespace Nc
{
    namespace Graphic
    {
        template<typename VisitorType, bool IsConst = false, typename ReturnType = void>
        class SceneNodeVisitor : public Graph::NodeVisitor<VisitorType, ReturnType, System::Object>
        {
            public:
                template<typename ToVisitList, typename VisitableList>
                SceneNodeVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, Graph::VisitTarget visitTarget = Graph::VisitChilds, bool postVisits = true, bool direction = true)
                    : Graph::NodeVisitor<VisitorType, ReturnType, System::Object>(toVisitList, visitableList, visitTarget, postVisits, direction)
                {}

                void InvokeChilds(Entity &n)
                {
                    for (Entity::ContainerType::iterator it = n.Childs().begin(); it != n.Childs().end(); ++it)
                        (*this)(**it);
                    if (n.Data != NULL)
                        (*this)(*n.Data);
                }

                void InvokeChilds(Octree &n)
                {
                    for (unsigned int i = 0; i < 8; ++i)
                    {
                        if (n.Childs().Data[i] != NULL)
                            (*this)(*n.Childs().Data[i]);
                    }
                    for (std::list<ISceneNode*>::iterator it = n.Data.begin(); it != n.Data.end(); ++it)
                        (*this)(**it);
                }
        };

        template<typename VisitorType, typename ReturnType>
        class SceneNodeVisitor<VisitorType, true, ReturnType> : public Graph::NodeVisitor<VisitorType, ReturnType, const System::Object>
        {
            public:
                template<typename ToVisitList, typename VisitableList>
                SceneNodeVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, Graph::VisitTarget visitTarget = Graph::VisitChilds, bool postVisits = true, bool direction = true)
                    : Graph::NodeVisitor<VisitorType, ReturnType, const System::Object>(toVisitList, visitableList, visitTarget, postVisits, direction)
                {}

                void InvokeChilds(const Entity &n)
                {
                    for (Entity::ContainerType::const_iterator it = n.Childs().begin(); it != n.Childs().end(); ++it)
                        (*this)(**it);
                    if (n.Data != NULL)
                        (*this)(*n.Data);
                }

                void InvokeChilds(const Octree &n)
                {
                    for (unsigned int i = 0; i < 8; ++i)
                    {
                        if (n.Childs().Data[i] != NULL)
                            (*this)(*n.Childs().Data[i]);
                    }
                    for (std::list<ISceneNode*>::const_iterator it = n.Data.begin(); it != n.Data.end(); ++it)
                        (*this)(**it);
                }
        };
    }
}

#endif
