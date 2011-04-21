
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

#include "Extension.h"

#ifdef SYSTEM_WINDOWS
    #include <GL/wglext.h>
#else
    #include <GL/glx.h>
#endif


using namespace Nc::Graphic::GL;

Extension::Extension()
{
    _supportNonPowerOfTwoTexture = false;
/*
    MultTransposeMatrixd = NULL;
    MultTransposeMatrixf = NULL;

    TexImage3D = NULL;
    ActiveTexture = NULL;
    Uniform1i = NULL;
    Uniform4f = NULL;
    UniformMatrix4fv = NULL;

    GenBuffers = NULL;
    BindBuffer = NULL;
    BufferData = NULL;
    BufferSubData = NULL;
    DeleteBuffers = NULL;
    MapBuffer = NULL;
    UnmapBuffer = NULL;
    VertexAttribPointer = NULL;
    EnableVertexAttribArray = NULL;
    DisableVertexAttribArray = NULL;

    DeleteShader = NULL;
    DeleteProgram = NULL;
    UseProgram = NULL;
    CreateShader = NULL;
    ShaderSource = NULL;
    CompileShader = NULL;
    CreateProgram = NULL;
    AttachShader = NULL;
    LinkProgram = NULL;
    GetUniformLocation = NULL;
    GetAttribLocation = NULL;
    GetShaderiv = NULL;
    GetShaderInfoLog = NULL;
    GetProgramInfoLog = NULL;
    GetProgramiv = NULL;
*/
#ifdef SYSTEM_WINDOWS
    wglChoosePixelFormat = NULL;
    wglCreatePbuffer = NULL;
    wglGetPbufferDC = NULL;
#endif
}

Extension::~Extension()
{
}

bool Extension::Init()
{
    bool r = true;

    _supportNonPowerOfTwoTexture = IsSupported("GL_ARB_texture_non_power_of_two");
/*
    r = (r && (MultTransposeMatrixd = (MULTTRANSPOSEMATRIXD_PROC)LoadFunc("glMultTransposeMatrixd")) != NULL);
    r = (r && (MultTransposeMatrixf = (MULTTRANSPOSEMATRIXF_PROC)LoadFunc("glMultTransposeMatrixf")) != NULL);
    r = (r && (UniformMatrix4fv = (UNIFORMMATRIX4FV_PROC)LoadFunc("glUniformMatrix4fv")) != NULL);
    r = (r && (Uniform4f = (UNIFORM4F_PROC)LoadFunc("glUniform4f")) != NULL);

// Texture
    r = (r && (TexImage3D = (TEXIMAGE3D_PROC)LoadFunc("glTexImage3D")) != NULL);
    r = (r && (ActiveTexture = (ACTIVETEXTURE_PROC)LoadFunc("glActiveTexture")) != NULL);
    r = (r && (Uniform1i = (UNIFORM1I_PROC)LoadFunc("glUniform1i")) != NULL);

// Buffer
    r = (r && (GenBuffers = (GENBUFFERS_PROC)LoadFunc("glGenBuffers")) != NULL);
    r = (r && (BindBuffer = (BINDBUFFER_PROC)LoadFunc("glBindBuffer")) != NULL);
    r = (r && (BufferData = (BUFFERDATA_PROC)LoadFunc("glBufferData")) != NULL);
    r = (r && (BufferSubData = (BUFFERSUBDATA_PROC)LoadFunc("glBufferSubData")) != NULL);
    r = (r && (DeleteBuffers = (DELETEBUFFERS_PROC)LoadFunc("glDeleteBuffers")) != NULL);
    r = (r && (MapBuffer = (MAPBUFFER_PROC)LoadFunc("glMapBuffer")) != NULL);
    r = (r && (UnmapBuffer = (UNMAPBUFFER_PROC)LoadFunc("glUnmapBuffer")) != NULL);
    r = (r && (VertexAttribPointer = (VERTEXATTRIBPOINTER_PROC)LoadFunc("glVertexAttribPointer")) != NULL);
    r = (r && (EnableVertexAttribArray = (ENABLEVERTEXATTRIBARRAY_PROC)LoadFunc("glEnableVertexAttribArray")) != NULL);
    r = (r && (DisableVertexAttribArray = (DISABLEVERTEXATTRIBARRAY_PROC)LoadFunc("glDisableVertexAttribArray")) != NULL);

// shader
    r = (r && (DeleteShader = (DELETESHADER_PROC)LoadFunc("glDeleteShader")) != NULL);
    r = (r && (DeleteProgram = (DELETEPROGRAM_PROC)LoadFunc("glDeleteProgram")) != NULL);
    r = (r && (UseProgram = (USEPROGRAM_PROC)LoadFunc("glUseProgram")) != NULL);
    r = (r && (CreateShader = (CREATESHADER_PROC)LoadFunc("glCreateShader")) != NULL);
    r = (r && (ShaderSource = (SHADERSOURCE_PROC)LoadFunc("glShaderSource")) != NULL);
    r = (r && (CompileShader = (COMPILESHADER_PROC)LoadFunc("glCompileShader")) != NULL);
    r = (r && (CreateProgram = (CREATEPROGRAM_PROC)LoadFunc("glCreateProgram")) != NULL);
    r = (r && (AttachShader = (ATTACHSHADER_PROC)LoadFunc("glAttachShader")) != NULL);
    r = (r && (LinkProgram = (LINKPROGRAM_PROC)LoadFunc("glLinkProgram")) != NULL);
    r = (r && (GetUniformLocation = (GETUNIFORMLOCATION_PROC)LoadFunc("glGetUniformLocation")) != NULL);
    r = (r && (GetAttribLocation = (GETATTRIBLOCATION_PROC)LoadFunc("glGetAttribLocation")) != NULL);
    r = (r && (GetShaderiv = (GETSHADERIV_PROC)LoadFunc("glGetShaderiv")) != NULL);
    r = (r && (GetShaderInfoLog = (GETSHADERINFOLOG_PROC)LoadFunc("glGetShaderInfoLog")) != NULL);
    r = (r && (GetProgramInfoLog = (GETPROGRAMINFOLOG_PROC)LoadFunc("glGetProgramInfoLog")) != NULL);
    r = (r && (GetProgramiv = (GETPROGRAMIV_PROC)LoadFunc("glGetProgramiv")) != NULL);
*/
/// wgl
#ifdef SYSTEM_WINDOWS
    r = (r && (wglChoosePixelFormat = (WGLCHOOSEPIXELFORMAT_PROC)LoadFunc("wglChoosePixelFormatARB")) != NULL);
    r = (r && (wglCreatePbuffer = (WGLCREATEPBUFFER_PROC)LoadFunc("wglCreatePbufferARB")) != NULL);
    r = (r && (wglGetPbufferDC = (WGLGETPBUFFERDC_PROC)LoadFunc("wglGetPbufferDCARB")) != NULL);
#endif
    return r;
}

void *Extension::LoadFunc(const char *functionName)
{
    void *r;
    #ifdef SYSTEM_WINDOWS
        r = (void*)wglGetProcAddress(functionName);
    #elif defined(SYSTEM_MACOS)
        r = (void*)NSGLGetProcAddress(functionName);
    #else
        r = (void*)glXGetProcAddress((const GLubyte*)functionName);
    #endif
    if (r == NULL)
        LOG_ERROR << "Can't load function `" << functionName << "`" << std::endl;
    return r;
}


bool    Extension::IsSupported(const char *extName)
{
    const char      *exts = (const char*)(GetInfo(GL_EXTENSIONS));
    bool            r = false;
    unsigned int    j = 0;

    if (exts == NULL)
        throw Utils::Exception("Exception", "Failed to get the supported extention list !");
    if (extName[j] == '\0') // false, if the extName is empty
        return false;

    for (unsigned int i = 0; exts[i] != '\0' && !r; ++i)
    {
        // search for marching the extName
        j = 0;
        for (; exts[i] == extName[j] && extName[j] != '\0'; ++i, ++j)
            if (extName[j] == ' ') // false, if the extName contain a space ' '
                return false;
        if (extName[j] == '\0')
            r = true;

        // go to the next space ' '
        for (; exts[i] != ' ' && exts[i] != '\0' && !r; ++i);
    }
    return r;
}

const GLubyte *Extension::GetInfo(unsigned int type)
{
    const GLubyte   *s = glGetString(type);

    if (s == NULL)
        throw Utils::Exception("GLContext", "glGetString return NULL");
    return s;
}

