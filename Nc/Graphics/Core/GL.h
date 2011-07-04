
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_CORE_GL_H_
#define NC_GRAPHIC_CORE_GL_H_

    //#define GL3_PROTOTYPES
    //#include <GL3/gl3.h>

	//#include <GL/glu.h>
    //#include <GL/glext.h>

// define if we are on windows the extentions
#ifdef SYSTEM_WINDOWS
	// buffer
	#define glGenVertexArrays				EXT.glGenVertexArrays
	#define glEnableVertexAttribArray		EXT.glEnableVertexAttribArray
	#define glDisableVertexAttribArray		EXT.glDisableVertexAttribArray
	#define glBindVertexArray				EXT.glBindVertexArray
	#define glDeleteVertexArrays			EXT.glDeleteVertexArrays
	#define glVertexAttribPointer			EXT.glVertexAttribPointer
	#define glDeleteBuffers					EXT.glDeleteBuffers
	#define glBufferData					EXT.glBufferData
	#define glBufferSubData					EXT.glBufferSubData
	#define glGenBuffers					EXT.glGenBuffers
	#define glMapBuffer						EXT.glMapBuffer
	#define glUnmapBuffer					EXT.glUnmapBuffer
	#define glBindBuffer					EXT.glBindBuffer
	#define glDrawArrays					EXT.glDrawArrays
	#define glDrawElements					EXT.glDrawElements

	// texture
	#define glActiveTexture			EXT.glActiveTexture
	#define glTexImage3D			EXT.glTexImage3D
	#define glGenerateMipmap		EXT.glGenerateMipmap

	// shader
	#define glAttachShader			EXT.glAttachShader
	#define glLinkProgram			EXT.glLinkProgram
	#define glGetProgramiv			EXT.glGetProgramiv
	#define glGetShaderiv			EXT.glGetShaderiv
	#define glGetProgramInfoLog		EXT.glGetProgramInfoLog
	#define glGetShaderInfoLog		EXT.glGetShaderInfoLog
	#define glCreateProgram			EXT.glCreateProgram
	#define glCreateShader			EXT.glCreateShader
	#define glCompileShader			EXT.glCompileShader
	#define glShaderSource			EXT.glShaderSource
	#define glDeleteShader			EXT.glDeleteShader
	#define glDeleteProgram			EXT.glDeleteProgram
	#define glUseProgram			EXT.glUseProgram

	// uniform
	#define glUniform1i				EXT.glUniform1i
	#define glUniform4f				EXT.glUniform4f
	#define glUniformMatrix4fv		EXT.glUniformMatrix4fv
	#define glGetUniformLocation	EXT.glGetUniformLocation

	// attrib
	#define glBindAttribLocation	EXT.glBindAttribLocation
	#define glGetAttribLocation		EXT.glGetAttribLocation
#endif

    #include "../Define.h"
	#include "GL/Extension.h"
	#include "GL/Enum.h"

#endif
