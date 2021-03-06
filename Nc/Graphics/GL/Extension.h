
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

    File Created At:        15/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_EXTENSION_H_
#define NC_GRAPHICS_GL_EXTENSION_H_

#include "../Define.h"

#define GL3_PROTOTYPES
#include <GL3/gl3.h>

#ifdef SYSTEM_WINDOWS
	#define WGL_WGLEXT_PROTOTYPES
	#include <GL3/wglext.h>
#endif

// calling convention
#ifndef CALL_GLEXT
#  if defined(__CYGWIN__) || defined(__MINGW32__)
#    define CALL_GLEXT __stdcall
#  elif (_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED) || defined(__BORLANDC__)
#    define CALL_GLEXT __stdcall
#  else
#    define CALL_GLEXT
#  endif
#endif

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Manage the OpenGL extensions.
            /**
                The Extension class is especialy used on windows to dynamically load opengl functions.
                But it also provide methods to load and determine if an opengl extention is supported.

                \todo On windows, GDebugger drop an error like "function load into a context and used into an other". So we should load function for a given context. Our best option is to remove the singleton and put the extention class as a member of GL::State or WGL/GLXContext.
            */
            class LIB_NC_GRAPHICS Extension : public Utils::Singleton<Extension>
            {
                public:
                    virtual ~Extension();

                    /** Init and load the extensions. */
                    bool Init();

                    /** Return true if the given extension name is support. */
                    bool    IsSupported(const char *extName);
                    /** Return true if non power of 2 texture is supported. */
                    bool    NonPowerOf2Supported()              {return _supportNonPowerOfTwoTexture;}

                protected:
                    Extension();

                    /** Load the given function. */
                    void *LoadFunc(const char *functionName);

                    bool        _supportNonPowerOfTwoTexture;   ///< true if we support non power of 2 texture.

                #ifdef SYSTEM_WINDOWS
				public:
					// buffer
					PFNGLGENVERTEXARRAYSPROC				glGenVertexArrays;
					PFNGLENABLEVERTEXATTRIBARRAYPROC		glEnableVertexAttribArray;
					PFNGLDISABLEVERTEXATTRIBARRAYPROC		glDisableVertexAttribArray;
					PFNGLBINDVERTEXARRAYPROC				glBindVertexArray;
					PFNGLDELETEVERTEXARRAYSPROC				glDeleteVertexArrays;
					PFNGLVERTEXATTRIBPOINTERPROC			glVertexAttribPointer;
					PFNGLDELETEBUFFERSPROC					glDeleteBuffers;
					PFNGLBUFFERDATAPROC						glBufferData;
					PFNGLBUFFERSUBDATAPROC					glBufferSubData;
					PFNGLGENBUFFERSPROC						glGenBuffers;
					PFNGLMAPBUFFERPROC						glMapBuffer;
					PFNGLUNMAPBUFFERPROC					glUnmapBuffer;
					PFNGLBINDBUFFERPROC						glBindBuffer;
					PFNGLDRAWARRAYSPROC						glDrawArrays;
					PFNGLDRAWELEMENTSPROC					glDrawElements;
					PFNGLGENFRAMEBUFFERSPROC				glGenFramebuffers;
					PFNGLDELETEFRAMEBUFFERSPROC				glDeleteFramebuffers;
					PFNGLBINDFRAMEBUFFERPROC				glBindFramebuffer;
					PFNGLFRAMEBUFFERRENDERBUFFERPROC		glFramebufferRenderbuffer;
					PFNGLFRAMEBUFFERTEXTURE2DPROC			glFramebufferTexture2D;
					PFNGLCHECKFRAMEBUFFERSTATUSPROC			glCheckFramebufferStatus;
					PFNGLBLITFRAMEBUFFERPROC				glBlitFramebuffer;
					PFNGLGENRENDERBUFFERSPROC				glGenRenderbuffers;
					PFNGLDELETERENDERBUFFERSPROC			glDeleteRenderbuffers;
					PFNGLBINDRENDERBUFFERPROC				glBindRenderbuffer;
					PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC	glRenderbufferStorageMultisample;
					PFNGLRENDERBUFFERSTORAGEPROC			glRenderbufferStorage;

					// texture
					PFNGLACTIVETEXTUREPROC			glActiveTexture;
					PFNGLTEXIMAGE3DPROC				glTexImage3D;
					PFNGLGENERATEMIPMAPPROC			glGenerateMipmap;

					// shader
					PFNGLATTACHSHADERPROC			glAttachShader;
					PFNGLLINKPROGRAMPROC			glLinkProgram;
					PFNGLGETPROGRAMIVPROC			glGetProgramiv;
					PFNGLGETSHADERIVPROC			glGetShaderiv;
					PFNGLGETPROGRAMINFOLOGPROC		glGetProgramInfoLog;
					PFNGLGETSHADERINFOLOGPROC		glGetShaderInfoLog;
					PFNGLCREATEPROGRAMPROC			glCreateProgram;
					PFNGLCREATESHADERPROC			glCreateShader;
					PFNGLCOMPILESHADERPROC			glCompileShader;
					PFNGLSHADERSOURCEPROC			glShaderSource;
					PFNGLDELETESHADERPROC			glDeleteShader;
					PFNGLDELETEPROGRAMPROC			glDeleteProgram;
					PFNGLUSEPROGRAMPROC				glUseProgram;

					// uniform
					PFNGLGETUNIFORMFVPROC			glGetUniformfv;
					PFNGLUNIFORM1IPROC				glUniform1i;
					PFNGLUNIFORM1UIPROC				glUniform1ui;
					PFNGLUNIFORM1FPROC				glUniform1f;
					PFNGLUNIFORM1FVPROC				glUniform1fv;
					PFNGLUNIFORM2FPROC				glUniform2f;
					PFNGLUNIFORM3FPROC				glUniform3f;
					PFNGLUNIFORM4FPROC				glUniform4f;
					PFNGLUNIFORMMATRIX4FVPROC		glUniformMatrix4fv;
					PFNGLGETUNIFORMLOCATIONPROC		glGetUniformLocation;

					// attrib
					PFNGLBINDATTRIBLOCATIONPROC		glBindAttribLocation;
					PFNGLGETATTRIBLOCATIONPROC		glGetAttribLocation;

					// fb output
					PFNGLBINDFRAGDATALOCATIONPROC	glBindFragDataLocation;
					PFNGLDRAWBUFFERSPROC			glDrawBuffers;

                    DECLARE_HANDLE(HPBUFFERARB);

				private:
                    typedef bool (CALL_GLEXT *WGLCHOOSEPIXELFORMAT_PROC)(HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
                    typedef ::HPBUFFERARB (CALL_GLEXT *WGLCREATEPBUFFER_PROC)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
                    typedef HDC (CALL_GLEXT *WGLGETPBUFFERDC_PROC)(::HPBUFFERARB hPbuffer);

                public:
                    WGLCHOOSEPIXELFORMAT_PROC		wglChoosePixelFormat;       ///< to choose a pixel format in Win32 System.
                    WGLCREATEPBUFFER_PROC			wglCreatePbuffer;           ///< to create a pbuffer in Win32 System.
                    WGLGETPBUFFERDC_PROC			wglGetPbufferDC;            ///< to get a DC pbuffer in Win32 System.
                #endif

                friend class Utils::Singleton<Extension>;
            };
        }
    }

    // explicit instantiation
	template class Utils::Singleton<Graphic::GL::Extension>;         ///< explicit instanciation of the extention singleton.
}

#endif
