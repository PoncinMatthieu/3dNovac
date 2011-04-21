
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

#ifndef GLIGHT_H_INCLUDED
#define GLIGHT_H_INCLUDED

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a light especially using with the DefaultMaterialPolitic class
        /**
            Just define a position, a color and a radius for the light
            \todo Manage different types of lights (sun, spot, etc...)
        */
        class LGRAPHICS Light : public Object3d
        {
            public:
            // constructeur
                Light();
                Light(const Vector3f &p, const Color &c, const float radius = 10);

                /** copy the light */
                virtual inline Object3d     *Clone() const          {return new Light(*this);}

                //void Draw(ISceneGraph *scene);

            // accesseurs
                /** Return the position of the light */
                Vector3f    &Position()                 {return _positionLight;}
                /** Return the color of the light */
                Color       &ColorRGB()                 {return _colorLight;}
                /** Return the raduis of the light */
                float       Radius()                    {return _radius;}

                /** Set the position of the light */
                void Position(const Vector3f &V)        {_positionLight=V;}
                /** Set the color of the light */
                void ColorRGB(Color &C)                 {_colorLight=C;}
                /** Set the raduis of the light */
                void Radius(float f)                    {_radius=f;}

            protected:
                Vector3f    _positionLight;             ///< the position of the light
                Color       _colorLight;                ///< the color of the light
                float       _radius;                    ///< the raduis of the light
        };
    }
}

#endif
