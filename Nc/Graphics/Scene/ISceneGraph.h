
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

#ifndef NC_CORE_SYSTEM_ISCENEGRAPH_H_
#define NC_CORE_SYSTEM_ISCENEGRAPH_H_

#include <stack>
#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface used by the SceneGraphManager to store data of a scene and render it
        /**
            A SceneGraph is basically composed by 3 stack of matrix used to render the scene. <br/>
            So there are : <br/>
                * the ProjectionMatrix      <-- Used to project the model like in a perspective or in a orthonormal repere  <br/>
                * the ViewMatrix            <-- Used to Set the View (camera) in the scene                                  <br/>
                * the ModelMatrix           <-- Used to locate the model in the repere                                      <br/>
        */
        class LGRAPHICS ISceneGraph : public System::Object, Utils::NonCopyable
        {
            public:
                ISceneGraph()
                {
                    _stackProjectionMatrix.push(TMatrix::Identity);
                    _stackViewMatrix.push(TMatrix::Identity);
                    _stackModelMatrix.push(TMatrix::Identity);
                }
                virtual ~ISceneGraph()      {}

                /** To update the projection and view matrix */
                virtual void    SetCurrentScene() = 0;

                /** Render the scene */
                virtual void    Render() = 0;

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

            private:
                std::stack<TMatrix>     _stackProjectionMatrix;     ///< the stack of projection matrix
                std::stack<TMatrix>     _stackViewMatrix;           ///< the stack of view matrix
                std::stack<TMatrix>     _stackModelMatrix;          ///< the stack of model matrix
        };
    }
}

#endif // NC_CORE_SYSTEM_ENGINERENDERER_H_
