
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
	glUniform1ui = NULL;


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
	glUniform1i = NULL;
	glUniform4f = NULL;
	glUniformMatrix4fv = NULL;
	glGetUniformLocation = NULL;

	// attrib
	glBindAttribLocation = NULL;
	glGetAttribLocation = NULL;

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
	r = (r && (glGenVertexArrays = (PFNGLGENVERTEXARRAYSPROC)LoadFunc("glGenVertexArrays")) != NULL);
	r = (r && (glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)LoadFunc("glEnableVertexAttribArray")) != NULL);
	r = (r && (glDisableVertexAttribArray = (PFNGLDISABLEVERTEXATTRIBARRAYPROC)LoadFunc("glDisableVertexAttribArray")) != NULL);
	r = (r && (glBindVertexArray = (PFNGLBINDVERTEXARRAYPROC)LoadFunc("glBindVertexArray")) != NULL);
	r = (r && (glDeleteVertexArrays = (PFNGLDELETEVERTEXARRAYSPROC)LoadFunc("glDeleteVertexArrays")) != NULL);
	r = (r && (glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)LoadFunc("glVertexAttribPointer")) != NULL);
	r = (r && (glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)LoadFunc("glDeleteBuffers")) != NULL);
	r = (r && (glBufferData = (PFNGLBUFFERDATAPROC)LoadFunc("glBufferData")) != NULL);
	r = (r && (glBufferSubData = (PFNGLBUFFERSUBDATAPROC)LoadFunc("glBufferSubData")) != NULL);
	r = (r && (glGenBuffers = (PFNGLGENBUFFERSPROC)LoadFunc("glGenBuffers")) != NULL);
	r = (r && (glMapBuffer = (PFNGLMAPBUFFERPROC)LoadFunc("glMapBuffer")) != NULL);
	r = (r && (glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)LoadFunc("glUnmapBuffer")) != NULL);
	r = (r && (glBindBuffer = (PFNGLBINDBUFFERPROC)LoadFunc("glBindBuffer")) != NULL);
	r = (r && (glDrawArrays = (PFNGLDRAWARRAYSPROC)LoadFunc("glDrawArrays")) != NULL);
	r = (r && (glDrawElements = (PFNGLDRAWELEMENTSPROC)LoadFunc("glDrawElements")) != NULL);
	r = (r && (glGenFramebuffers = (PFNGLGENFRAMEBUFFERSPROC)LoadFunc("glGenFramebuffers")) != NULL);
	r = (r && (glDeleteFramebuffers = (PFNGLDELETEFRAMEBUFFERSPROC)LoadFunc("glDeleteFramebuffers")) != NULL);
	r = (r && (glBindFramebuffer = (PFNGLBINDFRAMEBUFFERPROC)LoadFunc("glBindFramebuffer")) != NULL);
	r = (r && (glFramebufferRenderbuffer = (PFNGLFRAMEBUFFERRENDERBUFFERPROC)LoadFunc("glFramebufferRenderbuffer")) != NULL);
	r = (r && (glFramebufferTexture = (PFNGLFRAMEBUFFERTEXTUREPROC)LoadFunc("glFramebufferTexture")) != NULL);
	r = (r && (glCheckFramebufferStatus = (PFNGLCHECKFRAMEBUFFERSTATUSPROC)LoadFunc("glCheckFramebufferStatus")) != NULL);
	r = (r && (glBlitFramebuffer = (PFNGLBLITFRAMEBUFFERPROC)LoadFunc("glBlitFramebuffer")) != NULL);
	r = (r && (glGenRenderbuffers = (PFNGLGENRENDERBUFFERSPROC)LoadFunc("glGenRenderbuffers")) != NULL);
	r = (r && (glDeleteRenderbuffers = (PFNGLDELETERENDERBUFFERSPROC)LoadFunc("glDeleteRenderbuffers")) != NULL);
	r = (r && (glBindRenderbuffer = (PFNGLBINDRENDERBUFFERPROC)LoadFunc("glBindRenderbuffer")) != NULL);
	r = (r && (glRenderbufferStorageMultisample = (PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC)LoadFunc("glRenderbufferStorageMultisample")) != NULL);
	r = (r && (glRenderbufferStorage = (PFNGLRENDERBUFFERSTORAGEPROC)LoadFunc("glRenderbufferStorage")) != NULL);
	r = (r && (glUniform1ui = (PFNGLUNIFORM1UIPROC)LoadFunc("glUniform1ui")) != NULL);

	// texture
	r = (r && (glActiveTexture = (PFNGLACTIVETEXTUREPROC)LoadFunc("glActiveTexture")) != NULL);
	r = (r && (glTexImage3D = (PFNGLTEXIMAGE3DPROC)LoadFunc("glTexImage3D")) != NULL);
	r = (r && (glGenerateMipmap = (PFNGLGENERATEMIPMAPPROC)LoadFunc("glGenerateMipmap")) != NULL);

	// shader
	r = (r && (glAttachShader = (PFNGLATTACHSHADERPROC)LoadFunc("glAttachShader")) != NULL);
	r = (r && (glLinkProgram = (PFNGLLINKPROGRAMPROC)LoadFunc("glLinkProgram")) != NULL);
	r = (r && (glGetProgramiv = (PFNGLGETPROGRAMIVPROC)LoadFunc("glGetProgramiv")) != NULL);
	r = (r && (glGetShaderiv = (PFNGLGETSHADERIVPROC)LoadFunc("glGetShaderiv")) != NULL);
	r = (r && (glGetProgramInfoLog = (PFNGLGETPROGRAMINFOLOGPROC)LoadFunc("glGetProgramInfoLog")) != NULL);
	r = (r && (glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)LoadFunc("glGetShaderInfoLog")) != NULL);
	r = (r && (glCreateProgram = (PFNGLCREATEPROGRAMPROC)LoadFunc("glCreateProgram")) != NULL);
	r = (r && (glCreateShader = (PFNGLCREATESHADERPROC)LoadFunc("glCreateShader")) != NULL);
	r = (r && (glCompileShader = (PFNGLCOMPILESHADERPROC)LoadFunc("glCompileShader")) != NULL);
	r = (r && (glShaderSource = (PFNGLSHADERSOURCEPROC)LoadFunc("glShaderSource")) != NULL);
	r = (r && (glDeleteShader = (PFNGLDELETESHADERPROC)LoadFunc("glDeleteShader")) != NULL);
	r = (r && (glDeleteProgram = (PFNGLDELETEPROGRAMPROC)LoadFunc("glDeleteProgram")) != NULL);
	r = (r && (glUseProgram = (PFNGLUSEPROGRAMPROC)LoadFunc("glUseProgram")) != NULL);

	// uniform
	r = (r && (glUniform1i = (PFNGLUNIFORM1IPROC)LoadFunc("glUniform1i")) != NULL);
	r = (r && (glUniform4f = (PFNGLUNIFORM4FPROC)LoadFunc("glUniform4f")) != NULL);
	r = (r && (glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)LoadFunc("glUniformMatrix4fv")) != NULL);
	r = (r && (glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)LoadFunc("glGetUniformLocation")) != NULL);

	// attrib
	r = (r && (glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)LoadFunc("glBindAttribLocation")) != NULL);
	r = (r && (glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)LoadFunc("glGetAttribLocation")) != NULL);

	/// wgl
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

std::string   Extension::GetError()
{
    GLenum e = glGetError();
    switch (e)
    {
        case GL_NO_ERROR:                       return "No error has been recorded";
        case GL_INVALID_ENUM:                   return "An unacceptable value is specified for an enumerated argument";
        case GL_INVALID_VALUE:                  return "A numeric argument is out of range";
        case GL_INVALID_OPERATION:              return "The specified operation is not allowed in the current state";
        case GL_INVALID_FRAMEBUFFER_OPERATION:  return "The framebuffer object is not complete";
        case GL_OUT_OF_MEMORY:                  return "There is not enough memory left to execute the command";
        default:                                return "error";
    }
    return "";
}
