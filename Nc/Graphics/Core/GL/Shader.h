
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

    File Created At:        16/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_SHADER_H_
#define NC_GRAPHICS_CORE_GL_SHADER_H_

#include <string>
#include <map>
#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Help to compile and use a glsl program shader
            /**
                Actually manage only Vertex and Fragment shader in glsl
            */
            class LGRAPHICS Shader : public Object
            {
                private:
                    typedef std::map<const char*,unsigned int>      MapIndex;

                public:
                    Shader();
                    Shader(const std::string &vertexFile, const std::string &fragmentFile);
                    virtual ~Shader();

                    /** Load Vertex and Fragment shader from files */
                    void                    LoadFromFiles(const std::string &vertexFile, const std::string &fragmentFile);
                    /** Load Vertex and Fragment shader from memory */
                    void                    LoadFromMemory(const char *vertexSource, const char *fragmentSource, const std::string &name);

                    /** Enable the shader */
                    virtual inline void     Enable() const      {glUseProgram(_program);}
                    /** Disable the shader */
                    virtual inline void     Disable() const     {glUseProgram(0);}
                    /** Return the index of the shader */
                    virtual unsigned int    GetIndex() const    {return _program;}

                    /** Return the given uniform location */
                    unsigned int            UniformLocation(const char *name);
                    /** Return the given attribute location */
                    unsigned int            AttribLocation(const char *name);
                    /** Bind the attribute */
                    void                    BindAttrib(unsigned int attrib, const char *name);

                private:
                    /** Release the program shader */
                    virtual void    Release();

                    // shader       // type = GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER
                    /** Create a new shader */
                    unsigned int    NewShader(const Utils::FileName &filename, GLenum type); // renvoie 0 si ca echoue
                    /** Compile the given shader */
                    unsigned int    CompileShader(const char *source, GLenum type, const std::string &name);

                    // programme shader
                    /** Create a new program shader */
                    unsigned int    NewProgramShader(); // renvoie 0 si ca echoue
                    /** Attach and link the two shader */
                    void            AttachAndLinkProgram(unsigned int p, unsigned int vertexShader, unsigned int fragmentShader);

                    unsigned int    _program;           ///< the program shader
                    unsigned int    _vertexShader;      ///< the vertex shader
                    unsigned int    _fragmentShader;    ///< the fragment shader
            };
        }
    }
}

#endif
