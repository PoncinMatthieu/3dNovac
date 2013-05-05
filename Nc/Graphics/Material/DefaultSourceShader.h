
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

    File Created At:        05/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_MATERIAL_DEFAULTSOURCES_H_
#define NC_GRAPHICS_MATERIAL_DEFAULTSOURCES_H_

#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        /// Reference the sources of the shaders used with the DefaultMaterial and BasicVertexType.
        namespace DefaultSources
        {
            extern LIB_NC_GRAPHICS const char *ColoredVertexShader;               ///< Default Vertex shader to render a Colored VertexType.
            extern LIB_NC_GRAPHICS const char *ColoredFragmentShader;             ///< Default Fragment shader to render a Colored VertexType.

            extern LIB_NC_GRAPHICS const char *Colored2dVertexShader;             ///< Default Vertex shader to render a Colored2d VertexType.
            extern LIB_NC_GRAPHICS const char *Colored2dFragmentShader;           ///< Default Fragment shader to render a Colored2d VertexType.

            extern LIB_NC_GRAPHICS const char *TexturedVertexShader;              ///< Default Vertex shader to render a Textured VertexType.
            extern LIB_NC_GRAPHICS const char *TexturedFragmentShader;            ///< Default Fragment shader to render a Textured VertexType.

            extern LIB_NC_GRAPHICS const char *Textured2dVertexShader;            ///< Default Vertex shader to render a Textured2d VertexType.
            extern LIB_NC_GRAPHICS const char *Textured2dFragmentShader;          ///< Default Fragment shader to render a Textured2d VertexType.

            extern LIB_NC_GRAPHICS const char *Textured3dVertexShader;            ///< Default Vertex shader to render a Textured3d VertexType.
            extern LIB_NC_GRAPHICS const char *Textured3dFragmentShader;          ///< Default Fragment shader to render a Textured3d VertexType.
        }
    }
}

#endif
