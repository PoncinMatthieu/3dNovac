
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

    File Created At:        21/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_RASTERMODE_H_
#define NC_GRAPHICS_CORE_GL_RASTERMODE_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To manipulate the poligon rasterization mode
            /**
                \todo Manage the glLineWidth, glPointSize, glPolygonOffset, glHint (for antialiasing), glFog functions

                Abstract the glPolygonMode witch controls the interpretation of polygons for rasterization.

                from the OpenGL documumentation "http://www.opengl.org/sdk/docs/man3/":

                Three modes are defined and can be specified in mode:
                - GL_POINT

                Polygon vertices that are marked as the start of a boundary edge are drawn as points.
                Point attributes such as GL_POINT_SIZE and GL_POINT_SMOOTH control the rasterization of the points.
                Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.

                - GL_LINE

                Boundary edges of the polygon are drawn as line segments.
                Line attributes such as GL_LINE_WIDTH and GL_LINE_SMOOTH control the rasterization of the lines.
                Polygon rasterization attributes other than GL_POLYGON_MODE have no effect.

                - GL_FILL

                The interior of the polygon is filled.
                Polygon attributes such as GL_POLYGON_SMOOTH control the rasterization of the polygon.
            */
            class RasterMode
            {
                public:
                    RasterMode()
                        : _face(GL_FRONT_AND_BACK), _mode(GL_FILL) {}
                    RasterMode(GLenum face, GLenum mode)
                        : _face(face), _mode(mode) {}

                    /** Enable the given polygon rasterization mode */
                    inline void Enable()           {glPolygonMode(_face, _mode);}
                    /** Disable the given polygon rasterization mode by activating the default rasterization mode */
                    inline void Disable()          {glPolygonMode(_face, GL_FILL);}

                    /** Set the polygon resterization mode */
                    inline void SetRasterMode(GLenum face, GLenum mode)    {_face = face; _mode = mode;}

                private:
                    /** Must be GL_FRONT_AND_BACK for front- and back-facing polygons. */
                    GLenum  _face;  ///< Specifies the polygons that mode applies

                    /** Accepted values are GL_POINT, GL_LINE, and GL_FILL. The initial value is GL_FILL for both front- and back-facing polygons. */
                    GLenum  _mode;  ///< Specifies how polygons will be rasterized
            };
        }
    }
}

#endif
