
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
    #include <GL3/wglext.h>
#else
    #include <GL/glx.h>
#endif

#define LOAD_FUNC(r, func, type)					(r = (r && (func = (type)LoadFunc(#func)) != NULL))

using namespace Nc::Graphic::GL;

Extension::Extension()
{
    _supportNonPowerOfTwoTexture = false;

#ifdef SYSTEM_WINDOWS
	// buffer
	glGenVertexArrays = NULL;
	glEnableVertexAttribArray = NULL;
	glDisableVertexAttribArray = NULL;
	glBindVertexArray = NULL;
	glDeleteVertexArrays = NULL;
	glVertexAttribPointer = NULL;
	glDeleteBuffers = NULL;
	glBufferData = NULL;
	glBufferSubData = NULL;
	glGenBuffers = NULL;
	glMapBuffer = NULL;
	glUnmapBuffer = NULL;
	glBindBuffer = NULL;
	glDrawArrays = NULL;
	glDrawElements = NULL;
	glGenFramebuffers = NULL;
	glDeleteFramebuffers = NULL;
	glBindFramebuffer = NULL;
	glFramebufferRenderbuffer = NULL;
	glFramebufferTexture = NULL;
	glCheckFramebufferStatus = NULL;
	glBlitFramebuffer = NULL;
	glGenRenderbuffers = NULL;
	glDeleteRenderbuffers = NULL;
	glBindRenderbuffer = NULL;
	glRenderbufferStorageMultisample = NULL;
	glRenderbufferStorage = NULL;

	// texture
	glActiveTexture = NULL;
	glTexImage3D = NULL;
	glGenerateMipmap = NULL;

	// shader
	glAttachShader = NULL;
	glLinkProgram = NULL;
	glGetProgramiv = NULL;
	glGetShaderiv = NULL;
	glGetProgramInfoLog = NULL;
	glGetShaderInfoLog = NULL;
	glCreateProgram = NULL;
	glCreateShader = NULL;
	glCompileShader = NULL;
	glShaderSource = NULL;
	glDeleteShader = NULL;
	glDeleteProgram = NULL;
	glUseProgram = NULL;

	// uniform
	glGetUniformfv = NULL;
	glUniform1i = NULL;
	glUniform1ui = NULL;
	glUniform1f = NULL;
	glUniform1fv = NULL;
	glUniform2f = NULL;
	glUniform3f = NULL;
	glUniform4f = NULL;
	glUniformMatrix4fv = NULL;
	glGetUniformLocation = NULL;

	// attrib
	glBindAttribLocation = NULL;
	glGetAttribLocation = NULL;

	// fb output
	glBindFragDataLocation = NULL;
	glDrawBuffers = NULL;

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

#ifdef SYSTEM_WINDOWS
	// buffer
	LOAD_FUNC(r, glGenVertexArrays, PFNGLGENVERTEXARRAYSPROC);
	LOAD_FUNC(r, glEnableVertexAttribArray, PFNGLENABLEVERTEXATTRIBARRAYPROC);
	LOAD_FUNC(r, glDisableVertexAttribArray, PFNGLDISABLEVERTEXATTRIBARRAYPROC);
	LOAD_FUNC(r, glBindVertexArray, PFNGLBINDVERTEXARRAYPROC);
	LOAD_FUNC(r, glDeleteVertexArrays, PFNGLDELETEVERTEXARRAYSPROC);
	LOAD_FUNC(r, glVertexAttribPointer, PFNGLVERTEXATTRIBPOINTERPROC);
	LOAD_FUNC(r, glDeleteBuffers, PFNGLDELETEBUFFERSPROC);
	LOAD_FUNC(r, glBufferData, PFNGLBUFFERDATAPROC);
	LOAD_FUNC(r, glBufferSubData, PFNGLBUFFERSUBDATAPROC);
	LOAD_FUNC(r, glGenBuffers, PFNGLGENBUFFERSPROC);
	LOAD_FUNC(r, glMapBuffer, PFNGLMAPBUFFERPROC);
	LOAD_FUNC(r, glUnmapBuffer, PFNGLUNMAPBUFFERPROC);
	LOAD_FUNC(r, glBindBuffer, PFNGLBINDBUFFERPROC);
//	LOAD_FUNC(r, glDrawArrays, PFNGLDRAWARRAYSPROC);
	glDrawArrays = ::glDrawArrays;
//	LOAD_FUNC(r, glDrawElements, PFNGLDRAWELEMENTSPROC);
	glDrawElements = ::glDrawElements;
	LOAD_FUNC(r, glGenFramebuffers, PFNGLGENFRAMEBUFFERSPROC);
	LOAD_FUNC(r, glDeleteFramebuffers, PFNGLDELETEFRAMEBUFFERSPROC);
	LOAD_FUNC(r, glBindFramebuffer, PFNGLBINDFRAMEBUFFERPROC);
	LOAD_FUNC(r, glFramebufferRenderbuffer, PFNGLFRAMEBUFFERRENDERBUFFERPROC);
	LOAD_FUNC(r, glFramebufferTexture, PFNGLFRAMEBUFFERTEXTUREPROC);
	LOAD_FUNC(r, glCheckFramebufferStatus, PFNGLCHECKFRAMEBUFFERSTATUSPROC);
	LOAD_FUNC(r, glBlitFramebuffer, PFNGLBLITFRAMEBUFFERPROC);
	LOAD_FUNC(r, glGenRenderbuffers, PFNGLGENRENDERBUFFERSPROC);
	LOAD_FUNC(r, glDeleteRenderbuffers, PFNGLDELETERENDERBUFFERSPROC);
	LOAD_FUNC(r, glBindRenderbuffer, PFNGLBINDRENDERBUFFERPROC);
	LOAD_FUNC(r, glRenderbufferStorageMultisample, PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC);
	LOAD_FUNC(r, glRenderbufferStorage, PFNGLRENDERBUFFERSTORAGEPROC);

	// texture
	LOAD_FUNC(r, glActiveTexture, PFNGLACTIVETEXTUREPROC);
	LOAD_FUNC(r, glTexImage3D, PFNGLTEXIMAGE3DPROC);
	LOAD_FUNC(r, glGenerateMipmap, PFNGLGENERATEMIPMAPPROC);

	// shader
	LOAD_FUNC(r, glAttachShader, PFNGLATTACHSHADERPROC);
	LOAD_FUNC(r, glLinkProgram, PFNGLLINKPROGRAMPROC);
	LOAD_FUNC(r, glGetProgramiv, PFNGLGETPROGRAMIVPROC);
	LOAD_FUNC(r, glGetShaderiv, PFNGLGETSHADERIVPROC);
	LOAD_FUNC(r, glGetProgramInfoLog, PFNGLGETPROGRAMINFOLOGPROC);
	LOAD_FUNC(r, glGetShaderInfoLog, PFNGLGETSHADERINFOLOGPROC);
	LOAD_FUNC(r, glCreateProgram, PFNGLCREATEPROGRAMPROC);
	LOAD_FUNC(r, glCreateShader, PFNGLCREATESHADERPROC);
	LOAD_FUNC(r, glCompileShader, PFNGLCOMPILESHADERPROC);
	LOAD_FUNC(r, glShaderSource, PFNGLSHADERSOURCEPROC);
	LOAD_FUNC(r, glDeleteShader, PFNGLDELETESHADERPROC);
	LOAD_FUNC(r, glDeleteProgram, PFNGLDELETEPROGRAMPROC);
	LOAD_FUNC(r, glUseProgram, PFNGLUSEPROGRAMPROC);

	// uniform
	LOAD_FUNC(r, glGetUniformfv, PFNGLGETUNIFORMFVPROC);
	LOAD_FUNC(r, glUniform1i, PFNGLUNIFORM1IPROC);
	LOAD_FUNC(r, glUniform1ui, PFNGLUNIFORM1UIPROC);
	LOAD_FUNC(r, glUniform1f, PFNGLUNIFORM1FPROC);
	LOAD_FUNC(r, glUniform1fv, PFNGLUNIFORM1FVPROC);
	LOAD_FUNC(r, glUniform2f, PFNGLUNIFORM2FPROC);
	LOAD_FUNC(r, glUniform3f, PFNGLUNIFORM3FPROC);
	LOAD_FUNC(r, glUniform4f, PFNGLUNIFORM4FPROC);
	LOAD_FUNC(r, glUniformMatrix4fv, PFNGLUNIFORMMATRIX4FVPROC);
	LOAD_FUNC(r, glGetUniformLocation, PFNGLGETUNIFORMLOCATIONPROC);

	// attrib
	LOAD_FUNC(r, glBindAttribLocation, PFNGLBINDATTRIBLOCATIONPROC);
	LOAD_FUNC(r, glGetAttribLocation, PFNGLGETATTRIBLOCATIONPROC);

	// fb output
	LOAD_FUNC(r, glBindFragDataLocation, PFNGLBINDFRAGDATALOCATIONPROC);
	LOAD_FUNC(r, glDrawBuffers, PFNGLDRAWBUFFERSPROC);

	/// wgl
    r = (r && (wglChoosePixelFormat = (WGLCHOOSEPIXELFORMAT_PROC)LoadFunc("wglChoosePixelFormatARB")) != NULL);
    r = (r && (wglCreatePbuffer = (WGLCREATEPBUFFER_PROC)LoadFunc("wglCreatePbufferARB")) != NULL);
    r = (r && (wglGetPbufferDC = (WGLGETPBUFFERDC_PROC)LoadFunc("wglGetPbufferDCARB")) != NULL);
#endif
    return r;
}

void *Extension::LoadFunc(const char *functionName)
{
    void *r = NULL;
    #ifdef SYSTEM_WINDOWS
        r = (void*)wglGetProcAddress((LPCSTR)functionName);
    #elif defined(SYSTEM_MACOS)
        r = (void*)NSGLGetProcAddress(functionName);
    #else
        r = (void*)glXGetProcAddress((const GLubyte*)functionName);
    #endif
    if (r == NULL)
        LOG_ERROR << "Can't load function `" << functionName << "`" << std::endl;
    return r;
}

#include "State.h"

bool    Extension::IsSupported(const char *extName)
{
    const char      *exts = (const char*)(State::GetString(Enum::Extensions));
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
