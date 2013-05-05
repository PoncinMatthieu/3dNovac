
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
#include "../Effect/RasterEffect.h"
#include "SceneNode.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface used by the SceneGraphManager to store data of a scene and render it.
        /**
            A Scene Graph is basically composed by 3 stack of matrix used to render the scene.

            The Scene Graph has also a RasterMode used to globally set the default rasters mode.
            <div class="title"> So there are : </div>
                - the ProjectionMatrix      <-- Used to project the model like in a perspective or in a orthonormal repere.
                - the ViewMatrix            <-- Used to Set the View (camera) in the scene.
                - the ModelMatrix           <-- Used to locate the model in the repere.
        */
        class LIB_NC_GRAPHICS SceneGraph : public Entity
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Entity, Nc::System::Object, Nc::Graphic::SceneGraph);

            public:
                SceneGraph(bool for2dRendering = false);
                virtual ~SceneGraph();

                virtual ISceneNode      *Clone() const              {return new SceneGraph(*this);}

                /** \return the current projection matrix. */
                inline TMatrix          &ProjectionMatrix()         {return _stackProjectionMatrix.top();}
                /** \return the current projection matrix. */
                inline const TMatrix    &ProjectionMatrix() const   {return _stackProjectionMatrix.top();}
                /** Push in the stack the projection matrix. */
                inline void             PushProjectionMatrix()      {_stackProjectionMatrix.push(_stackProjectionMatrix.top());}
                /** Unstack the projection matrix. */
                inline void             PopProjectionMatrix()       {_stackProjectionMatrix.pop();}

                /** \return the current view matrix. */
                inline TMatrix          &ViewMatrix()               {return _stackViewMatrix.top();}
                /** \return the current view matrix. */
                inline const TMatrix    &ViewMatrix() const         {return _stackViewMatrix.top();}
                /** Push in the stack the view matrix. */
                inline void             PushViewMatrix()            {_stackViewMatrix.push(_stackViewMatrix.top());}
                /** Unstack the view matrix. */
                inline void             PopViewMatrix()             {_stackViewMatrix.pop();}

                /** \return the current model matrix. */
                inline TMatrix          &ModelMatrix()              {return _stackModelMatrix.top();}
                /** \return the current model matrix. */
                inline const TMatrix    &ModelMatrix() const        {return _stackModelMatrix.top();}
                /** Push in the stack the model matrix. */
                inline void             PushModelMatrix()           {_stackModelMatrix.push(_stackModelMatrix.top());}
                /** Unstack the model matrix. */
                inline void             PopModelMatrix()            {_stackModelMatrix.pop();}

                /**
                    If there is no material, the object will used their default materials.
                    \return the current rendering material, if there is no material return null.
                */
                inline IMaterial            *&Material()                {return _stackMaterials.top();}
                /**
                    If there is no material, the object will used their default materials.
                    \return the current rendering material, if there is no material return null.
                */
                inline IMaterial            *const&Material() const     {return _stackMaterials.top();}
                /** Push in the stack the model matrix. */
                inline void                 PushMaterial()              {_stackMaterials.push(NULL);}
                /** Unstack the model matrix. */
                inline void                 PopMaterial()               {_stackMaterials.pop();}

                /** \return the raster mode of the scene. */
                inline const RasterEffect   &GetRasterEffect() const    {return _rasterEffect;}
                /** \return the raster mode of the scene. */
                inline RasterEffect         &GetRasterEffect()          {return _rasterEffect;}

                /** \return the GL::State of the render thread. */
                inline const GL::State      *GLState() const            {return _oglState;}
                /** \return the GL::State of the render thread. */
                inline GL::State            *GLState()                  {return _oglState;}

                /**
                    To update the projection matrix with the camera.
                    If the camera is shared between multiple scene, then you will need to Setup the camera at each changement of scene.
                */
                void                SetupCamera();

                /** \return the current camera.  */
                inline Camera       *CurrentCamera()                    {return _currentCamera;}
                /** \return the current camera.  */
                inline Camera       *CurrentCamera() const              {return _currentCamera;}
                /** Set the current camera.  */
                inline void         CurrentCamera(Camera *cam)          {_currentCamera = cam;}

                /**
                    Compute and map object coordinates to window coordinates.
                    \return return true if succeed. compute and return the window coordinates from the object coordinates. If the currentCamera is null, the method will return false.
                */
                bool                    Project(const Vector3f &objCoords, Vector3f &winCoords) const;

            protected:
                /** Update the child nodes of the scene graph. */
                virtual void    Update(float elapsedTime)           {UpdateChilds(elapsedTime);}
                /** Render the scene. */
                virtual void    Render(SceneGraph *scene);

            protected:
                Camera                  *_currentCamera;            ///< the current camera of the scene (could be used by some node to produce a specific rendering by using the current camera).
                RasterEffect            _rasterEffect;              ///< the raster effect used to render the scene.

            private:
                GL::State               *_oglState;                 ///< store the current ogl state at the rendering time.
                std::stack<TMatrix>     _stackProjectionMatrix;     ///< the stack of projection matrix.
                std::stack<TMatrix>     _stackViewMatrix;           ///< the stack of view matrix.
                std::stack<TMatrix>     _stackModelMatrix;          ///< the stack of model matrix.
                std::stack<IMaterial*>  _stackMaterials;            ///< the stack of rendering material.
        };

        // specialization of the method GetNode<> for an IObject
        //template<>
        //SceneGraph *ISceneNode::GetNode<>();
    }
}

#endif
