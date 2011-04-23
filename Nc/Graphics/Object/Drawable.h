
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        04/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_OBJECT_DRAWABLE_H_
#define NC_GRAPHICS_OBJECT_DRAWABLE_H_

#include "../Material/Material.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to define a Drawable
        /**
            A drawable is composed by a MaterialConfig (by inheritance) and by a VertexType and a MaterialConfigPolitic (by template)
        */
        template<typename VertexType, typename MaterialConfigPolitic>
        struct IDrawable : public MaterialConfig<VertexType, MaterialConfigPolitic>
        {
            /** Return the associated geometry */
            virtual GL::IGeometryBuffer<VertexType>     &GetGeometry() = 0;

            /** Render the geometry of the drawable */
            virtual void                                Render() = 0;
        };

        /// Define a Drawable
        /**
            A drawable is composed by an IDrawable and also by a GeometryBuffer (by inheritance)
            The drawable class is typically here to simplify the manipulation of MaterialConfig and GeometryBuffer
        */
        template<typename VertexType, bool INDEX, typename MaterialConfigPolitic>
        struct Drawable : public IDrawable<VertexType, MaterialConfigPolitic>, public GL::GeometryBuffer<VertexType, INDEX>
        {
            Drawable &operator = (const GL::GeometryBuffer<VertexType, INDEX> &geometry)
            {
                static_cast<GL::GeometryBuffer<VertexType, INDEX>*>(this)->operator=(geometry);
            }
            Drawable &operator = (const MaterialConfig<VertexType, MaterialConfigPolitic> &config)
            {
                static_cast<MaterialConfig<VertexType, MaterialConfigPolitic>*>(this)->operator=(config);
                return *this;
            }

            /** Return the associated geometry */
            virtual GL::IGeometryBuffer<VertexType> &GetGeometry()
            {
                return *this;
            }

            /** Render the associated geometry */
            virtual void    Render()
            {
                GL::GeometryBuffer<VertexType, INDEX>::Render();
            }
        };
    }
}

#endif // NC_GRAPHICS_OBJECT_DRAWABLE_H_
