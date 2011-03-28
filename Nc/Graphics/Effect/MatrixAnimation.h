
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
/*-----------------------------------------------------------------------------

                    Implementation de la classe "gMatrixAnimation"
            permet la gestion d'animation simple par matrice sur un objet 3d

-----------------------------------------------------------------------------*/


#ifndef NOVAC_GRAPHIC_MATRIXANIMATION_H_
#define NOVAC_GRAPHIC_MATRIXANIMATION_H_

#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS   MatrixAnimation : public Effect
        {
            public:
                MatrixAnimation();
                MatrixAnimation(Object *o, const TMatrix &transformation = TMatrix());
                ~MatrixAnimation();

                virtual Effect  *Clone() const      {return new MatrixAnimation(*this);}

                virtual void    Start()             {}
                virtual void    Stop()              {}
                virtual bool    Started() const     {return true;}

                inline void     Reset()                     {if (_object != NULL) _object->Matrix = _initialMatrix;}
                virtual void    Update(float runningTime);
                virtual void    Render(ISceneGraph *scene)  {}


                // accesseurs
                inline void     SetObject(Object *o)                            {_object = o; _initialMatrix = _object->Matrix;}
    //            inline void   Transfornation(const mMatrice &m)   {_transformation = m;}    //TODO
                inline void     Rotation(const Vector3f &axe, float angle)      {_rotation = axe; _angle = angle;}

            private:
                Object          *_object;
                float           _angle;             // angle de rotation
                Vector3f        _rotation;          // vecteur de rotation, la matrice de tranformation n'est pas utiliser, il faudrai trouver un moyen d'utiliser le facteur temp
                TMatrix         _initialMatrix;
        };
    }
}

#endif

