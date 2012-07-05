
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

#ifndef NC_GRAPHICS_OBJECT_BOUNDING_BOX_H_
#define NC_GRAPHICS_OBJECT_BOUNDING_BOX_H_

#include "../Core/DefaultVertexType.h"
#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Basic 3d graphical object used to render an Axis Aligned Bounding Box (AABB)
        /**
            \todo put a reference counter here to destroy the static drawable of the box and avoid memory leek
        */
        class LGRAPHICS BoundingBox : public Object, public Box3f
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::Object, System::Object, Nc::Graphic::BoundingBox);

            public:
                BoundingBox();
                BoundingBox(const Box3f &box);
                BoundingBox(const Vector3f &min, const Vector3f &max);
                virtual ~BoundingBox();

                virtual ISceneNode      *Clone() const              {return new BoundingBox(*this);}

                virtual void            TransformModelMatrixToRender(SceneGraph *scene)    {}

                /** Render the box */
                void                    Draw(SceneGraph *scene);

                /** \return the height of the box */
                inline float            Height() const              {return Length(2);}

            private:
                /** Create the geometry of the box */
                static void             CreateGeometry();

                static Color            _color;         ///< the color of the box
                static Core::Drawable   *_drawable;     ///< the static drawable, static because we use the same drawable for all boxs
                static IDefaultMaterial *_material;     ///< the static material instance used to render the drawables
        };
    }
}

#endif
