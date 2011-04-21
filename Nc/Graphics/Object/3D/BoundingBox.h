
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

    File Created At:        02/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                Implementation de la classe abstraite "BoundingBox"

    permet de definir et de dessiner un bounding box

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_BOUNDING_BOX_
#define NOVAC_GRAPHIC_BOUNDING_BOX_

#include "Object3d.h"

namespace Nc
{
    namespace Graphic
    {
        /// Basic 3d graphical object to render a box
        class LGRAPHICS BoundingBox : public Object3d, public Box3f
        {
            public:
                BoundingBox();
                BoundingBox(const Box3f &box);
                BoundingBox(const Vector3f &min, const Vector3f &max);
                virtual ~BoundingBox();

                /** Render the box */
                void    Render(ISceneGraph *scene);

                /** Return the height of the box */
                inline float        Height() const          {return Length(2);}

            private:
                /** Create the geometry of the box */
                void    CreateGeometry();
                /** Update the geometry of the box */
                void    UpdateGeometry();

                Color   _color;         ///< the color of the box

                static Material<BasicVertexType::Colored>           *_material;     ///< the material in static because it's the same for all boxs
                static Drawable<BasicVertexType::Colored>           *_drawable;     ///< the drawable in static because it's the same for all boxs
        };
    }
}

#endif
