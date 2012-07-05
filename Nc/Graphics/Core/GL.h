
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

    #define VERSION_MIN_OPENGL  3.0

    #include "../Define.h"
	#include "GL/Extension.h"

// define if we are on windows the ogl functions
#ifdef SYSTEM_WINDOWS
	// buffer
	#define glGenVertexArrays					EXT.glGenVertexArrays
	#define glEnableVertexAttribArray			EXT.glEnableVertexAttribArray
	#define glDisableVertexAttribArray			EXT.glDisableVertexAttribArray
	#define glBindVertexArray					EXT.glBindVertexArray
	#define glDeleteVertexArrays				EXT.glDeleteVertexArrays
	#define glVertexAttribPointer				EXT.glVertexAttribPointer
	#define glDeleteBuffers						EXT.glDeleteBuffers
	#define glBufferData						EXT.glBufferData
	#define glBufferSubData						EXT.glBufferSubData
	#define glGenBuffers						EXT.glGenBuffers
	#define glMapBuffer							EXT.glMapBuffer
	#define glUnmapBuffer						EXT.glUnmapBuffer
	#define glBindBuffer						EXT.glBindBuffer
	#define glDrawArrays						EXT.glDrawArrays
	#define glDrawElements						EXT.glDrawElements
	#define glGenFramebuffers					EXT.glGenFramebuffers
	#define glDeleteFramebuffers				EXT.glDeleteFramebuffers
	#define glBindFramebuffer					EXT.glBindFramebuffer
	#define glFramebufferRenderbuffer			EXT.glFramebufferRenderbuffer
	#define glFramebufferTexture2D				EXT.glFramebufferTexture2D
	#define glCheckFramebufferStatus			EXT.glCheckFramebufferStatus
	#define glBlitFramebuffer					EXT.glBlitFramebuffer
	#define glGenRenderbuffers					EXT.glGenRenderbuffers
	#define glDeleteRenderbuffers				EXT.glDeleteRenderbuffers
	#define glBindRenderbuffer					EXT.glBindRenderbuffer
	#define glRenderbufferStorageMultisample	EXT.glRenderbufferStorageMultisample
	#define glRenderbufferStorage				EXT.glRenderbufferStorage

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
	#define glUniform1ui			EXT.glUniform1ui
	#define glUniform1f				EXT.glUniform1f
	#define glUniform1fv			EXT.glUniform1fv
	#define glUniform2f				EXT.glUniform2f
	#define glUniform3f				EXT.glUniform3f
	#define glUniform4f				EXT.glUniform4f
	#define glUniformMatrix4fv		EXT.glUniformMatrix4fv
	#define glGetUniformLocation	EXT.glGetUniformLocation

	// attrib
	#define glBindAttribLocation	EXT.glBindAttribLocation
	#define glGetAttribLocation		EXT.glGetAttribLocation

	// fb output
	#define glBindFragDataLocation	EXT.glBindFragDataLocation
	#define glDrawBuffers			EXT.glDrawBuffers
#endif

	#include "GL/Enum.h"
	#include "GL/State.h"

#endif
