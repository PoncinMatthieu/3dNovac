
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

#ifndef NC_GRAPHICS_SCENE_CONTROLLER_H_
#define NC_GRAPHICS_SCENE_CONTROLLER_H_

#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Base class to defines controllers in a scene node
        /**
            In truth, the Controller class is a specific visitor, which visit only the given node
        */
        template<typename ControllerType>
        class Controller : public Graph::NodeVisitor<ControllerType, void, System::Object>
        {
            public:
                template<typename ToVisitList, typename VisitableList>
                Controller(const ToVisitList &toVisitList, const VisitableList &visitableList)
                    : Graph::NodeVisitor<ControllerType, void, System::Object>(toVisitList, visitableList, Graph::VisitOnlyCurrentNode)
                {}

                void InvokeChilds(ISceneNode &n)                    {}
        };

        /// Interface to define a Rendering Controller
        /**
            This controller aims to be used at rendering time in a scene node.
            The property _scene is set just before calling the visit method.
            Pay attention that a rendering Controller is called twice at the rendering time once just before rendering and once just after, use the property _currentStade to know what is the current call.
        */
        class IRenderingController
        {
            public:
                enum Stade
                {
                    Begin,
                    End
                };

            public:
                IRenderingController()
                    : _scene(NULL), _currentStade(Begin) {}

                virtual void operator() (ISceneNode &n) = 0;

                inline void     Scene(SceneGraph *scene)                    {_scene = scene;}

                inline void     CurrentStade(Stade s)                       {_currentStade = s;}

            protected:
                SceneGraph      *_scene;
                Stade           _currentStade;
        };

        /// Interface to define an Updating Controller
        /**
            This controller aims to be used at updating time in a scene node.
            The property _elapsedTime is set just before calling the visit method.
        */
        class IUpdatingController
        {
            public:
                IUpdatingController()
                    : _elapsedTime(0) {}

                virtual void operator() (ISceneNode &n) = 0;

                void    ElapsedTime(float elapsedTime)              {_elapsedTime = elapsedTime;}

            protected:
                float   _elapsedTime;
        };
    }
}

#endif
