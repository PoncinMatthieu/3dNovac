
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

    File Created At:        28/02/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_SCENEGRAPH_H_
#define NC_CORE_SYSTEM_SCENEGRAPH_H_

#include <stack>
#include "../Define.h"
#include "../Core/GL/RasterMode.h"
#include "SceneNode.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface used by the SceneGraphManager to store data of a scene and render it
        /**
            A Scene Graph is basically composed by 3 stack of matrix used to render the scene. <br/>
            The Scene Graph has also a RasterMode used to globally set the default rasters mode.
            <div class="title"> So there are : </div>
                - the ProjectionMatrix      <-- Used to project the model like in a perspective or in a orthonormal repere
                - the ViewMatrix            <-- Used to Set the View (camera) in the scene
                - the ModelMatrix           <-- Used to locate the model in the repere
        */
        class LGRAPHICS SceneGraph : public Entity
        {
            public:
                SceneGraph(bool for2dRendering = false);
                SceneGraph(const char *className, bool for2dRendering = false);
                virtual ~SceneGraph();

                static const char       *ClassName()                {return "SceneGraph";}
                virtual ISceneNode      *Clone() const              {return new SceneGraph(*this);}

                /** Update the child nodes of the scene graph */
                virtual void    Update(float elapsedTime)           {if (_enabled) UpdateChilds(elapsedTime);}

                /** Render the scene */
                virtual void    Render(SceneGraph *)                {Render();}
                /** Render the scene */
                virtual void    Render();

                /** Return the current projection matrix */
                inline TMatrix  &ProjectionMatrix()                 {return _stackProjectionMatrix.top();}
                /** Push in the stack the projection matrix */
                inline void     PushProjectionMatrix()              {_stackProjectionMatrix.push(_stackProjectionMatrix.top());}
                /** Unstack the projection matrix */
                inline void     PopProjectionMatrix()               {_stackProjectionMatrix.pop();}

                /** Return the current view matrix */
                inline TMatrix  &ViewMatrix()                       {return _stackViewMatrix.top();}
                /** Push in the stack the view matrix */
                inline void     PushViewMatrix()                    {_stackViewMatrix.push(_stackViewMatrix.top());}
                /** Unstack the view matrix */
                inline void     PopViewMatrix()                     {_stackViewMatrix.pop();}

                /** Return the current model matrix */
                inline TMatrix  &ModelMatrix()                      {return _stackModelMatrix.top();}
                /** Push in the stack the model matrix */
                inline void     PushModelMatrix()                   {_stackModelMatrix.push(_stackModelMatrix.top());}
                /** Unstack the model matrix */
                inline void     PopModelMatrix()                    {_stackModelMatrix.pop();}

                /** \return the raster mode of the scene */
                inline GL::RasterMode   &RasterMode()               {return _mode;}

                /**
                    To update the projection matrix with the camera.
                    If the camera is shared between multiple scene, then you will need to Setup the camera at each changement of scene.
                */
                void            SetupCamera();

                /** \return the current camera  */
                inline Camera   *CurrentCamera()                    {return _currentCamera;}
                /** \return the current camera  */
                inline Camera   *CurrentCamera() const              {return _currentCamera;}
                /** Set the current camera  */
                inline void     CurrentCamera(Camera *cam)          {_currentCamera = cam;}

            protected:
                Camera                  *_currentCamera;            ///< the current camera of the scene (could be used by some node to produce a specific rendering by using the current camera)
                GL::RasterMode          _mode;                      ///< the raster mode used to render the scene

            private:
                std::stack<TMatrix>     _stackProjectionMatrix;     ///< the stack of projection matrix
                std::stack<TMatrix>     _stackViewMatrix;           ///< the stack of view matrix
                std::stack<TMatrix>     _stackModelMatrix;          ///< the stack of model matrix
        };

        // specialization of the method GetNode<> for an IObject
        //template<>
        //SceneGraph *ISceneNode::GetNode<>();
    }
}

#endif
