
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
        /** Interface used by the SceneGraphManager to store data of a scene and render it */
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

                virtual void    SetCurrentScene() = 0;
                virtual void    Render() = 0;

                inline TMatrix  &ProjectionMatrix()                 {return _stackProjectionMatrix.top();}
                inline void     PushProjectionMatrix()              {_stackProjectionMatrix.push(_stackProjectionMatrix.top());}
                inline void     PopProjectionMatrix()               {_stackProjectionMatrix.pop();}

                inline TMatrix  &ViewMatrix()                       {return _stackViewMatrix.top();}
                inline void     PushViewMatrix()                    {_stackViewMatrix.push(_stackViewMatrix.top());}
                inline void     PopViewMatrix()                     {_stackViewMatrix.pop();}

                inline TMatrix  &ModelMatrix()                      {return _stackModelMatrix.top();}
                inline void     PushModelMatrix()                   {_stackModelMatrix.push(_stackModelMatrix.top());}
                inline void     PopModelMatrix()                    {_stackModelMatrix.pop();}

            private:
                std::stack<TMatrix>     _stackProjectionMatrix;
                std::stack<TMatrix>     _stackViewMatrix;
                std::stack<TMatrix>     _stackModelMatrix;
        };
    }
}

#endif // NC_CORE_SYSTEM_ENGINERENDERER_H_
