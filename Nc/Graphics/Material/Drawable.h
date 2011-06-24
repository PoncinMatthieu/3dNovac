
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

#include "../Material/MaterialConfig.h"
#include "../Core/GL/GeometryBuffer.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a Drawable
        /**
            A drawable is an association of a MaterialConfig and a GeometryBuffer.
            The MaterialConfig help us to store data like Texture and RasterMode witch is material dependant
            and The Geometry help us to store data like vertices informations coord/colors/texture_coord witch is Material independant.
            The drawable class is typically here to simplify the manipulation of the MaterialConfig and GeometryBuffer with the materials.
        */
        struct LGRAPHICS Drawable
        {
            // constructs
            Drawable(GL::IGeometryBuffer *geometry);
            Drawable(GL::IGeometryBuffer *geometry, MaterialConfig *config);
            template<typename VertexType, unsigned int D1, unsigned int D2>
            Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, const Array<unsigned int,D2> &tabIndices, unsigned int stride, GLenum primitiveType);
            template<typename VertexType, unsigned int D1, unsigned int D2>
            Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, const Array<unsigned int,D2> &tabIndices, unsigned int stride, GLenum primitiveType, MaterialConfig *config);
            template<typename VertexType, unsigned int D1>
            Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, GLenum primitiveType);
            template<typename VertexType, unsigned int D1>
            Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, GLenum primitiveType, MaterialConfig *config);

            Drawable(const Drawable &d);
            Drawable &operator = (const Drawable &d);
            ~Drawable();

            // methods
            /** Enable the configs states before rendering */
            inline void                     Enable()                        {Config->Enable();}

            /** Disable the configs states after rendering */
            inline void                     Disable()                       {Config->Disable();}

            /** Render the associated geometry */
            inline void                     Render()                        {Geometry->Render();}

            // fields
            MaterialConfig          *Config;            ///< Pointer to a MaterialConfig or a sub class
            GL::IGeometryBuffer     *Geometry;          ///< Pointer to the geometry
        };

        template<typename VertexType, unsigned int D1, unsigned int D2>
        Drawable::Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, const Array<unsigned int,D2> &tabIndices, unsigned int stride, GLenum primitiveType)
        {
            Config = new MaterialConfig();
            Geometry = new GL::GeometryBuffer<VertexType, true>(tabVertices, flags, tabIndices, stride, primitiveType);
        }

        template<typename VertexType, unsigned int D1, unsigned int D2>
        Drawable::Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, const Array<unsigned int,D2> &tabIndices, unsigned int stride, GLenum primitiveType, MaterialConfig *config)
        {
            Config = config;
            Geometry = new GL::GeometryBuffer<VertexType, true>(tabVertices, flags, tabIndices, stride, primitiveType);
        }

        template<typename VertexType, unsigned int D1>
        Drawable::Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, GLenum primitiveType)
        {
            Config = new MaterialConfig();
            Geometry = new GL::GeometryBuffer<VertexType, false>(tabVertices, flags, primitiveType);
        }

        template<typename VertexType, unsigned int D1>
        Drawable::Drawable(const Array<VertexType,D1> &tabVertices, unsigned int flags, GLenum primitiveType, MaterialConfig *config)
        {
            Config = config;
            Geometry = new GL::GeometryBuffer<VertexType, false>(tabVertices, flags, primitiveType);
        }
    }
}

#endif
