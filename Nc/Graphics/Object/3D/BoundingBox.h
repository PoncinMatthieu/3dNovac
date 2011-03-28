
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
        class LGRAPHICS BoundingBox : public Object3d
        {
            public:
                BoundingBox();
                BoundingBox(const Box3f &box);
                BoundingBox(const Vector3f &min, const Vector3f &max);
                virtual ~BoundingBox();

                void    Render(ISceneGraph *scene);

                inline const Box3f  &GetBox() const         {return _box;}
                inline void         SetBox(const Box3f &b)  {_box = b;}
                inline float        Height() const          {return _box.Length(2);}

            private:
                void    CreateGeometry();
                void    UpdateGeometry();

                Box3f   _box;
                Color   _color;

                static Material<BasicVertexType::Colored>                   *_material;
                static MaterialConfig<BasicVertexType::Colored>             *_config;
                static GL::GeometryBuffer<BasicVertexType::Colored>         *_geometry;
        };
    }
}

#endif
