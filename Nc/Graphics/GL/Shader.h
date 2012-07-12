
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

#ifndef NC_GRAPHICS_GL_SHADER_H_
#define NC_GRAPHICS_GL_SHADER_H_

#include <string>
#include <map>
#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Help to compile and use a glsl shader.
            /**
                \sa
                    - Program

                \todo Create a compilation stage, for exemple to be able to manage GLSL version.
            */
            class LGRAPHICS Shader : public Object
            {
                public:
                    Shader();
                    Shader(const Utils::FileName &file, Enum::ShaderType type);
                    Shader(const char *source, Enum::ShaderType type, const Utils::FileName &name);
                    virtual ~Shader();

                    /** Create a new copy of the object by without duplicate the ogl ressources. */
                    virtual Object          *Clone() const              {return new Shader(*this);}

                    /** Return the index of the shader. */
                    virtual unsigned int    GetIndex() const            {return _shader;}

                    /** Load a shader from a file and compile it. */
                    void                    LoadFromFile(const Utils::FileName &file, Enum::ShaderType type);
                    /** Load a shader from memory and compile it. */
                    void                    LoadFromMemory(const char *source, Enum::ShaderType type, const Utils::FileName &name);

                private:
                    /** Release the program shader. */
                    virtual void    Release();

                    /** Compile the given shader source. */
                    void            Compile(const char *source, Enum::ShaderType type, const Utils::FileName &name);

					/** Print the compilation error. */
					void			PrintCompilationError(const char *source, Enum::ShaderType type, const Utils::FileName &name);

                private:
                    unsigned int    _shader;      ///< the shader id.
            };
        }
    }
}

#endif
