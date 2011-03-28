
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
/*-----------------------------------------------------------------------------

                        Implementation de la classe "gShader"
                   permet de gerer facilement un shader programme

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
            class LGRAPHICS Shader : public Object
            {
                private:
                    typedef std::map<const char*,unsigned int>      MapIndex;

                public:
                    Shader();
                    Shader(const std::string &vertexFile, const std::string &fragmentFile);
                    virtual ~Shader();

                    void                    LoadFromFiles(const std::string &vertexFile, const std::string &fragmentFile);
                    void                    LoadFromMemory(const char *vertexSource, const char *fragmentSource, const std::string &name);

                    virtual inline void     Enable() const      {glUseProgram(_program);}
                    virtual inline void     Disable() const     {glUseProgram(0);}
                    virtual unsigned int    GetIndex() const    {return _program;}
                    unsigned int            Program()           {return GetIndex();}

                    unsigned int            Uniform(const char *name);
                    unsigned int            Attrib(const char *name);
                    void                    BindAttrib(unsigned int attrib, const char *name);

                private:
                    virtual void    Release();

                    // shader       // type = GL_VERTEX_SHADER ou GL_FRAGMENT_SHADER
                    unsigned int    NewShader(const Utils::FileName &filename, GLenum type); // renvoie 0 si ca echoue
                    unsigned int    CompileShader(const char *source, GLenum type, const std::string &name);

                    // programme shader
                    unsigned int    NewProgramShader(); // renvoie 0 si ca echoue
                    void            AttachAndLinkProgram(unsigned int p, unsigned int vertexShader, unsigned int fragmentShader);

                    unsigned int    _program;
                    unsigned int    _vertexShader;
                    unsigned int    _fragmentShader;
            };
        }
    }
}

#endif
