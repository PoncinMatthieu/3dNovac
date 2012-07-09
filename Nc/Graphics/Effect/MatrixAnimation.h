
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

    File Created At:        09/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_MATRIXANIMATION_H_
#define NOVAC_GRAPHIC_MATRIXANIMATION_H_

#include "Animation.h"

namespace Nc
{
    namespace Graphic
    {
        /// To manage simple animation with matrix like Rotation/Translation.
        /**
            \deprecated this class shouldn't be use, try to create a Controller to manipulate the matrix on your node.
            \todo this class should be recode or just deleted, no longer necessary.
        */
        class LGRAPHICS   MatrixAnimation : public Animation
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Animation, System::Object, Nc::Graphic::MatrixAnimation)

            public:
                MatrixAnimation();
                //MatrixAnimation(const TMatrix &transformation);
                ~MatrixAnimation();

                /** Copy the matrix animation. */
                virtual ISceneNode  *Clone() const      {return new MatrixAnimation(*this);}

                virtual void        Start()             {}
                virtual void        Stop()              {}
                virtual bool        Started() const     {return true;}

                /** Reset the anim with the initial matrix. */
                inline void         Reset()                     {_currentMatrix = TMatrix::Identity;}

                // accesseurs
    //            inline void   Transfornation(const mMatrice &m)   {_transformation = m;}    //TODO
                /** Program a rotatation animation. */
                inline void     Rotation(const Vector3f &axe, float angle)      {_rotation = axe; _angle = angle;}

            protected:
                /** Update the matrix animation. */
                virtual void        Update(float runningTime);
                /** Render the matrix animation. */
                virtual void        Render(SceneGraph *scene);

            private:
                TMatrix         _currentMatrix;
                float           _angle;             ///< Rotation angle.
                Vector3f        _rotation;          ///< Rotation vector. The matrix transformation is not used. We should find a way to use the time factor.
        };
    }
}

#endif

