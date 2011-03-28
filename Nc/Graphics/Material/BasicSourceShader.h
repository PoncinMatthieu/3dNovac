
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
/*-----------------------------------------------------------------------------

	Description : contient les sources des shaders de base

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_BASICSOURCESHADER_H_
#define NC_GRAPHIC_BASICSOURCESHADER_H_

#include "../Define.h"

namespace Nc
{
    namespace Graphic
    {
        namespace BasicSource
        {
            extern const char* LGRAPHICS ColoredVertexShader;
            extern const char* LGRAPHICS ColoredFragmentShader;

            extern const char* LGRAPHICS Colored2dVertexShader;
            extern const char* LGRAPHICS Colored2dFragmentShader;

            extern const char* LGRAPHICS TexturedVertexShader;
            extern const char* LGRAPHICS TexturedFragmentShader;

            extern const char* LGRAPHICS Textured2dVertexShader;
            extern const char* LGRAPHICS Textured2dFragmentShader;

            extern const char* LGRAPHICS Textured3dVertexShader;
            extern const char* LGRAPHICS Textured3dFragmentShader;
        }
    }
}

#endif // NC_GRAPHIC_SOURCEBASICSHADER_H_
