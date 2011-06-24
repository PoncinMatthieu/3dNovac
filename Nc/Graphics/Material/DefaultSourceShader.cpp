
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "DefaultSourceShader.h"

using namespace Nc::Graphic;

const char *DefaultSources::ColoredVertexShader =
"#version 330 core\n\
\
uniform mat4    MVPMatrix;\
\
in vec3 InCoord;\
in vec3 InColor;\
\
out Vertice\
{\
    vec3        Color;\
}   vertice;\
\
void main ()\
{\
	vertice.Color = InColor;\
	gl_Position = MVPMatrix * vec4(InCoord, 1.0);\
}";

const char *DefaultSources::ColoredFragmentShader =
"#version 330 core\n\
precision highp float;\
\
in Vertice\
{\
    vec3        Color;\
}   vertice;\
\
out vec4        FragmentColor;\
\
void main()\
{\
	FragmentColor = vec4(vertice.Color, 1.0);\
}";

const char *DefaultSources::Colored2dVertexShader =
"#version 330 core\n\
\
uniform mat4    MVPMatrix;\
\
in vec2 InCoord;\
in vec3 InColor;\
\
out Vertice\
{\
    vec3        Color;\
}   vertice;\
\
void main ()\
{\
	vertice.Color = InColor;\
	gl_Position = MVPMatrix * vec4(InCoord, 0.0, 1.0);\
}";

const char *DefaultSources::Colored2dFragmentShader =
"#version 330 core\n\
precision highp float;\
\
in Vertice\
{\
    vec3        Color;\
}   vertice;\
\
out vec4        FragmentColor;\
\
void main()\
{\
	FragmentColor = vec4(vertice.Color, 1.0);\
}";

const char *DefaultSources::TexturedVertexShader =
"#version 330 core\n\
\
uniform mat4    MVPMatrix;\
\
in vec3 InCoord;\
in vec2 InTexCoord;\
in vec3 InColor;\
\
out Vertice\
{\
    vec2        TexCoord;\
    vec3        Color;\
}   vertice;\
\
void main ()\
{\
	vertice.Color = InColor;\
	vertice.TexCoord = InTexCoord;\
	gl_Position = MVPMatrix * vec4(InCoord, 1.0);\
}";

const char *DefaultSources::TexturedFragmentShader =
"#version 330 core\n\
precision highp float;\
\
uniform sampler2D   TextureId;\
uniform bool        Textured;\
\
in Vertice\
{\
    vec2            TexCoord;\
    vec3            Color;\
}   vertice;\
\
out vec4            FragmentColor;\
\
void main()\
{\
    if (Textured)\
        FragmentColor = texture(TextureId, vertice.TexCoord) * vec4(vertice.Color, 1.0);\
    else\
        FragmentColor = vec4(vertice.Color, 1.0);\
}";

const char *DefaultSources::Textured2dVertexShader =
"#version 330 core\n\
\
uniform mat4    MVPMatrix;\
\
in vec2         InCoord;\
in vec2         InTexCoord;\
in vec3         InColor;\
\
out Vertice\
{\
    vec2        TexCoord;\
    vec3        Color;\
}   vertice;\
\
void main ()\
{\
	vertice.Color = InColor;\
	vertice.TexCoord = InTexCoord;\n\
	gl_Position = MVPMatrix * vec4(InCoord, 0.0, 1.0);\
}";

const char *DefaultSources::Textured2dFragmentShader =
"#version 330 core\n\
precision highp float;\
\
uniform bool        Textured;\
uniform sampler2D   TextureId;\
\
in Vertice\
{\
    vec2            TexCoord;\
    vec3            Color;\
}   vertice;\
\
out vec4            FragmentColor;\
\
void main()\
{\
    if (Textured)\
        FragmentColor = texture(TextureId, vertice.TexCoord) * vec4(vertice.Color, 1.0);\
    else\
        FragmentColor = vec4(vertice.Color, 1.0);\
}";

const char *DefaultSources::Textured3dVertexShader =
"#version 330 core\n\
\
uniform mat4    MVPMatrix;\
\
in vec3         InCoord;\
in vec3         InTexCoord;\
in vec3         InColor;\
\
out Vertice\
{\
    vec3        TexCoord;\
    vec3        Color;\
}   vertice;\
\
void main ()\
{\
	vertice.Color = InColor;\
	vertice.TexCoord = InTexCoord;\
	gl_Position = MVPMatrix * vec4(InCoord, 1.0);\
}";

const char *DefaultSources::Textured3dFragmentShader =
"#version 330 core\n\
precision highp float;\
\
uniform bool        Textured;\
uniform samplerCube TextureId;\
\
in Vertice\
{\
    vec3            TexCoord;\
    vec3            Color;\
}   vertice;\
\
out vec4            FragmentColor;\
\
void main()\
{\
    if (Textured)\
        FragmentColor = texture(TextureId, vertice.TexCoord) * vec4(vertice.Color, 1.0);\
    else\
        FragmentColor = vec4(vertice.Color, 1.0);\
}";
