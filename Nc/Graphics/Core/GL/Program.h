
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

    File Created At:        03/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_PROGRAM_H_
#define NC_GRAPHICS_CORE_GL_PROGRAM_H_

#include "Shader.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Help to use a programe shader
            /**
                A program must be attached to shaders and linked before using it.

                \sa
                    - Shader
            */
            class Program : public Object
            {
                public:
                    Program();
                    ~Program();

                    /** Create a new copy of the object by without duplicate the ogl ressources */
                    virtual Object          *Clone() const              {return new Program(*this);}

                    /** Enable the program */
                    virtual void            Enable() const;
                    /** Disable the program */
                    virtual void            Disable() const;
                    /** Return the index of the program */
                    virtual unsigned int    GetIndex() const            {return _program;}

                    /** Attach a new shader to the program */
                    void                    Attach(const Shader &shader);
                    /** Attach a new shader source file to the program */
                    void                    Attach(const Utils::FileName &shaderFile, Enum::ShaderType type);
                    /** Attach a new shader source file to the program */
                    void                    Attach(const char *shaderSource, Enum::ShaderType type, const Utils::FileName &shaderName);

                    /** Link the program */
                    void                    Link();

                    /** Fetch the uniform location into the program */
                    unsigned int            GetUniformLocation(const char *name);
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, const TMatrix &m, bool transpose = true)      {glUniformMatrix4fv(location, 1, transpose, m.Elements());}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, int v)                                        {glUniform1i(location, v);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, unsigned int v)                               {glUniform1ui(location, v);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, float v)                                      {glUniform1f(location, v);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, float f1, float f2)                           {glUniform2f(location, f1, f2);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, const Vector2f &v)                            {glUniform2f(location, v.Data[0], v.Data[1]);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, float f1, float f2, float f3)                 {glUniform3f(location, f1, f2, f3);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, const Vector3f &v)                            {glUniform3f(location, v.Data[0], v.Data[1], v.Data[2]);}
                    /** Set the given uniform \p index */
                    static void             SetUniform(unsigned int location, float f1, float f2, float f3, float f4)       {glUniform4f(location, f1, f2, f3, f4);}

                    /** Fetch the attrib location */
                    unsigned int            GetAttribLocation(const char *name);
                    /**
                        Bind the attribute with the given attrib id, and put it into the attrib location tab
                        \return the position of the attrib into the attrib location tab
                    */
                    void                    BindAttrib(unsigned int attrib, const char *name);

                private:
                    /** Release the program */
                    virtual void            Release();

                private:
                    unsigned int                _program;           ///< the program
                    bool                        _linked;            ///< true if the shader has been linked
            };
        }
    }
}

#endif
