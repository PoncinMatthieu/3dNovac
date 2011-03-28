
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

    File Created At:        30/04/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                    Implementation de la classe "Light"

                Classe permettant de definir une lumiere et d'effectuer
                            des calculs d'eclairages

-----------------------------------------------------------------------------*/

#ifndef GLIGHT_H_INCLUDED
#define GLIGHT_H_INCLUDED

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS Light : public Object3d
        {
            public:
            /// constructeur
                Light();
                Light(const Vector3f &p, const Color &c, const float radius = 10);

                virtual inline Object3d     *Clone() const          {return new Light(*this);}

                //void Draw(ISceneGraph *scene);

            /// accesseurs
                Vector3f    Position()                {return _positionLight;}
                Color       ColorRGB()                {return _colorLight;}
                float       Radius()                      {return _radius;}

                void Position(const Vector3f& V)    {_positionLight=V;}
                void ColorRGB(Color& C)         {_colorLight=C;}
                void Radius(float f)                {_radius=f;}

            protected:
            /// variables definissant la position, la couleur et le rayon maximum de la lumiere
                Vector3f    _positionLight;
                Color       _colorLight;
                float       _radius;
        };
    }
}

#endif
