
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

#ifndef NC_GRAPHICS_EFFECT_VIEWPORTEFFECT_H_
#define NC_GRAPHICS_EFFECT_VIEWPORTEFFECT_H_

#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        /// Like a camera, this effect allow you to manipulate the Viewport/Projection/View.
        /**
            \todo Create an example using the camera effect.
        */
        class LIB_NC_GRAPHICS CameraEffect : public Effect
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Effect, System::Object, Nc::Graphic::CameraEffect);

            public:
                CameraEffect();
                virtual ~CameraEffect();

                virtual ISceneNode      *Clone() const                  {return new CameraEffect(*this);}

                /** \return the update viewport statement. */
                inline bool             UpdateViewport()                {return _updateViewport;}
                /** Set the update viewport statement. */
                inline void             UpdateViewport(bool state)      {_updateViewport = state;}
                /** \return the viewport X corner. */
                inline unsigned int     ViewportX()                     {return _viewportX;}
                /** \return the viewport Y corner. */
                inline unsigned int     ViewportY()                     {return _viewportY;}
                /** \return the viewport Width. */
                inline unsigned int     ViewportWidth()                 {return _viewportWidth;}
                /** \return the viewport Height. */
                inline unsigned int     ViewportHeight()                {return _viewportHeight;}
                /** Will set the given viewport at the rendering, automatically set the viewport statement to true. */
                void                    Viewport(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight);

                /** \return the update projection statement. */
                inline bool             UpdateProjection()              {return _updateProjection;}
                /** Set the update projection statement. */
                inline void             UpdateProjection(bool state)    {_updateProjection = state;}
                /** \return the projection matrix which will be used to update the projection. */
                inline const TMatrix    &Projection()                   {return _projectionMatrix;}
                /** Will set the projection as given, automatically set the projection statement to true. */
                void                    Projection(float ratioAspect, float nearf, float farf, float fieldOfView);
                /** Will set the projection as a plane with the given corners, automatically set the projection statement to true. */
                void                    Projection2d(unsigned int viewportX, unsigned int viewportY, unsigned int viewportWidth, unsigned int viewportHeight);
                /** Will set the projection as a plane by using the viewport values, automatically set the projection statement to true. */
                void                    Projection2d();

                /** \return the update view statement. */
                inline bool             UpdateView()                    {return _updateView;}
                /** Set the update view statement. */
                inline void             UpdateView(bool state)          {_updateView = state;}
                /** \return the projection matrix which will be used to update the projection. */
                inline const TMatrix    &View()                         {return _viewMatrix;}
                /** Will set the view matrix with the 3 given vectors, automatically set the view statement to true. */
                void                    View(Vector3f eye, Vector3f center, Vector3f up);

            protected:
                virtual void            Render(SceneGraph *scene);

            private:
                bool                _updateViewport;            ///< if true, update the viewport.
                unsigned int        _viewportX;                 ///< the lower left x componant of the viewport rectangle.
                unsigned int        _viewportY;                 ///< the lower left y componant of the viewport rectangle.
                unsigned int        _viewportWidth;             ///< the width of the viewport.
                unsigned int        _viewportHeight;            ///< the height of the viewport.

                bool                _updateProjection;          ///< if true, update the projection.
                TMatrix             _projectionMatrix;

                bool                _updateView;                ///< if true, will update the view matrix.
                TMatrix             _viewMatrix;
        };
    }
}

#endif
