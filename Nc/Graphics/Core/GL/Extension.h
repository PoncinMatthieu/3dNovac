
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
/*-----------------------------------------------------------------------------

    manage the gl extension

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_EXTENSION_H_
#define NC_GRAPHICS_CORE_GL_EXTENSION_H_

#include "../GL.h"
#include "../../Define.h"

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
//#define CALL_GLEXT		__stdcall
/*
/// draw
#define GL_STATIC_DRAW              0x88E4
#define GL_STREAM_DRAW              0x88E0

/// buffer
#define GL_ELEMENT_ARRAY_BUFFER     0x8893
#define GL_ARRAY_BUFFER             0x8892
#define GL_READ_WRITE               0x88BA
#define GL_WRITE_ONLY               0x88B9

/// shader
#ifndef GL_SHADING_LANGUAGE_VERSION
#define GL_SHADING_LANGUAGE_VERSION 0x8B8C
#endif
#ifndef GL_VERTEX_SHADER
#define GL_VERTEX_SHADER            GL_VERTEX_SHADER_EXT
#endif
#ifndef GL_FRAGMENT_SHADER
#define GL_FRAGMENT_SHADER          GL_FRAGMENT_SHADER_ATI
#endif
#ifndef GL_COMPILE_STATUS
#define GL_COMPILE_STATUS           0x8B81
#endif
#ifndef GL_INFO_LOG_LENGTH
#define GL_INFO_LOG_LENGTH          0x8B84
#endif
#ifndef GL_LINK_STATUS
#define GL_LINK_STATUS              0x8B82
#endif
*/
namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            // TODO: les instance sont duplique d'une .so a l'autre en utilisant Utils::Singleton
            class LCORE Extension : public Utils::NonCopyable// : public Utils::Singleton<Extension>
            {
                public:
                    virtual ~Extension();

                    static Extension &Instance();
                    bool Init();

                    const GLubyte    *GetInfo(unsigned int type);

                    bool    IsSupported(const char *extName);
                    bool    NonPowerOf2Supported()              {return _supportNonPowerOfTwoTexture;}

                private:
                    Extension();

                    void *LoadFunc(const char *functionName);

                    bool        _supportNonPowerOfTwoTexture;
    /*
                /// matrix
                private:
                    typedef void (CALL_GLEXT *MULTTRANSPOSEMATRIXD_PROC)(const GLdouble m[16]);
                    typedef void (CALL_GLEXT *MULTTRANSPOSEMATRIXF_PROC)(const GLfloat m[16]);

                public:
                    MULTTRANSPOSEMATRIXD_PROC   MultTransposeMatrixd;
                    MULTTRANSPOSEMATRIXF_PROC   MultTransposeMatrixf;

                /// Texture
                private:
                    typedef void (CALL_GLEXT *TEXIMAGE3D_PROC)(GLenum target, GLint level, GLint internalFormat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
                    typedef void (CALL_GLEXT *ACTIVETEXTURE_PROC)(GLenum texture);
                    typedef void (CALL_GLEXT *UNIFORM1I_PROC)(GLint location, GLint v0);
                    typedef void (CALL_GLEXT *UNIFORM4F_PROC)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
                    typedef void (CALL_GLEXT *UNIFORMMATRIX4FV_PROC)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);

                public:
                    TEXIMAGE3D_PROC             TexImage3D;
                    ACTIVETEXTURE_PROC          ActiveTexture;
                    UNIFORM1I_PROC              Uniform1i;
                    UNIFORM4F_PROC              Uniform4f;
                    UNIFORMMATRIX4FV_PROC       UniformMatrix4fv;

                /// buffer
                private:
                    typedef void (CALL_GLEXT *GENBUFFERS_PROC)(GLsizei n, GLuint* buffers);
                    typedef void (CALL_GLEXT *BINDBUFFER_PROC)(GLenum target, GLuint buffer);
                    typedef void (CALL_GLEXT *BUFFERDATA_PROC)(GLenum target, ptrdiff_t size, const GLvoid* data, GLenum usage);
                    typedef void (CALL_GLEXT *BUFFERSUBDATA_PROC)(GLenum target, ptrdiff_t offset, ptrdiff_t size, const GLvoid* data);
                    typedef void (CALL_GLEXT *DELETEBUFFERS_PROC)(GLsizei n, const GLuint* buffers);
                    typedef void (CALL_GLEXT *VERTEXATTRIBPOINTER_PROC)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid* pointer);
                    typedef void (CALL_GLEXT *ENABLEVERTEXATTRIBARRAY_PROC)(GLuint);
                    typedef void (CALL_GLEXT *DISABLEVERTEXATTRIBARRAY_PROC)(GLuint);
                    typedef GLvoid *(CALL_GLEXT *MAPBUFFER_PROC)(GLenum target, GLenum access);
                    typedef GLboolean (CALL_GLEXT *UNMAPBUFFER_PROC)(GLenum target);

                public:
                    GENBUFFERS_PROC                 GenBuffers;
                    BINDBUFFER_PROC                 BindBuffer;
                    BUFFERDATA_PROC                 BufferData;
                    BUFFERSUBDATA_PROC              BufferSubData;
                    DELETEBUFFERS_PROC              DeleteBuffers;
                    MAPBUFFER_PROC                  MapBuffer;
                    UNMAPBUFFER_PROC                UnmapBuffer;
                    VERTEXATTRIBPOINTER_PROC        VertexAttribPointer;
                    ENABLEVERTEXATTRIBARRAY_PROC    EnableVertexAttribArray;
                    DISABLEVERTEXATTRIBARRAY_PROC   DisableVertexAttribArray;

                /// shader functions :
                private:
                    typedef void (CALL_GLEXT *DELETESHADER_PROC)(GLuint shader);
                    typedef void (CALL_GLEXT *DELETEPROGRAM_PROC)(GLuint program);
                    typedef void (CALL_GLEXT *USEPROGRAM_PROC)(GLuint program);
                    typedef GLuint (CALL_GLEXT *CREATESHADER_PROC)(GLenum type);
                    typedef void (CALL_GLEXT *SHADERSOURCE_PROC)(GLuint shader, GLsizei count, const char** strings, const GLint* lengths);
                    typedef void (CALL_GLEXT *COMPILESHADER_PROC)(GLuint shader);
                    typedef GLuint (CALL_GLEXT *CREATEPROGRAM_PROC)(void);
                    typedef void (CALL_GLEXT *ATTACHSHADER_PROC)(GLuint program, GLuint shader);
                    typedef void (CALL_GLEXT *LINKPROGRAM_PROC)(GLuint program);
                    typedef GLint (CALL_GLEXT *GETUNIFORMLOCATION_PROC)(GLuint program, const char* name);
                    typedef GLint (CALL_GLEXT *GETATTRIBLOCATION_PROC)(GLuint program, const char* name);
                    typedef void (CALL_GLEXT *GETSHADERIV_PROC)(GLuint shader, GLenum pname, GLint* param);
                    typedef void (CALL_GLEXT *GETSHADERINFOLOG_PROC)(GLuint shader, GLsizei bufSize, GLsizei* length, char* infoLog);
                    typedef void (CALL_GLEXT *GETPROGRAMINFOLOG_PROC)(GLuint program, GLsizei bufSize, GLsizei* length, char* infoLog);
                    typedef void (CALL_GLEXT *GETPROGRAMIV_PROC)(GLuint program, GLenum pname, GLint* param);

                public:
                    DELETESHADER_PROC           DeleteShader;
                    DELETEPROGRAM_PROC          DeleteProgram;
                    USEPROGRAM_PROC             UseProgram;
                    CREATESHADER_PROC           CreateShader;
                    SHADERSOURCE_PROC           ShaderSource;
                    COMPILESHADER_PROC          CompileShader;
                    CREATEPROGRAM_PROC          CreateProgram;
                    ATTACHSHADER_PROC           AttachShader;
                    LINKPROGRAM_PROC            LinkProgram;
                    GETUNIFORMLOCATION_PROC     GetUniformLocation;
                    GETATTRIBLOCATION_PROC      GetAttribLocation;
                    GETSHADERIV_PROC            GetShaderiv;
                    GETSHADERINFOLOG_PROC       GetShaderInfoLog;
                    GETPROGRAMINFOLOG_PROC      GetProgramInfoLog;
                    GETPROGRAMIV_PROC           GetProgramiv;
    */

                #ifdef SYSTEM_WINDOWS
                    DECLARE_HANDLE(HPBUFFERARB);
                private:
                    typedef bool (CALL_GLEXT *WGLCHOOSEPIXELFORMAT_PROC)(HDC hdc, const int* piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
                    typedef ::HPBUFFERARB (CALL_GLEXT *WGLCREATEPBUFFER_PROC)(HDC hDC, int iPixelFormat, int iWidth, int iHeight, const int* piAttribList);
                    typedef HDC (CALL_GLEXT *WGLGETPBUFFERDC_PROC)(::HPBUFFERARB hPbuffer);

                public:
                    WGLCHOOSEPIXELFORMAT_PROC   wglChoosePixelFormat;
                    WGLCREATEPBUFFER_PROC       wglCreatePbuffer;
                    WGLGETPBUFFERDC_PROC        wglGetPbufferDC;
                #endif

                private:
                    static Extension   *_instance;
            };
        }
    }
}

#endif
