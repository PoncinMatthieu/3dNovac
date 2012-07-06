
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

    File Created At:        21/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_RENDERINGCONTROLLER_H_
#define NC_GRAPHICS_SCENE_RENDERINGCONTROLLER_H_

#include "Controller.h"
#include "Entity.h"
#include "../SpacePartitioning/Octree.h"

namespace Nc
{
    namespace Graphic
    {
        /// Help to defines rendering controllers in a scene node.
        template<typename ControllerType>
        class RenderingController : public Controller<ControllerType>, public IRenderingController
        {
            public:
                template<typename ToVisitList, typename VisitableList>
                RenderingController(const ToVisitList &toVisitList, const VisitableList &visitableList)
                    : Controller<ControllerType>(toVisitList, visitableList)
                {}

                template<typename ToVisitList>
                RenderingController(const ToVisitList &toVisitList)
                    : Controller<ControllerType>(toVisitList, Utils::Metaprog::Seq<Graphic::Entity, Graphic::Octree>::Type())
                {}

                virtual void operator() (ISceneNode &n)             {Controller<ControllerType>::operator()(n);}
        };
    }
}

#endif
