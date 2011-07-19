
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

                    /** Init the size of the uniform location tab, used to initialize the uniforms witch would be used by the program */
                    inline void             InitNumberOfUniform(unsigned int nb)                                        {_tabUniform.reserve(nb);}
                    /**
                        Get the uniform id location with the given name, and store it in the tab of uniform.
                        \return the index position of the uniform id into the tab
                    */
                    unsigned int            UseUniformLocation(const char *name);
                    /** \return the uniform location by using the \p index position */
                    inline unsigned int     Uniform(unsigned int index)                                                 {return _tabUniform.at(index);}
                    /** Set the given uniform \p index */
                    inline void             SetUniform(unsigned int index, const TMatrix &m, bool transpose = true)     {glUniformMatrix4fv(_tabUniform.at(index), 1, transpose, m.Elements());}
                    /** Set the given uniform \p index */
                    inline void             SetUniform(unsigned int index, float f1, float f2, float f3, float f4)      {glUniform4f(_tabUniform.at(index), f1, f2, f3, f4);}
                    /** Set the given uniform \p index */
                    inline void             SetUniform(unsigned int index, int v)                                       {glUniform1i(_tabUniform.at(index), v);}

                    /** Init the size of the attrib location tab, used to initialize the attribs witch would be used by the program */
                    inline void             InitNumberOfAttrib(unsigned int nb)                                         {_tabAttrib.reserve(nb);}
                    /**
                        Get the attrib id location with the given name, and store it in the tab of attrib.
                        \return the index position of the attrib id into the tab
                    */
                    unsigned int            UseAttribLocation(const char *name);
                    /** \return the attrib location by using the \p index position */
                    inline unsigned int     Attrib(unsigned int index)                                                  {return _tabAttrib.at(index);}
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

                    std::vector<unsigned int>   _tabUniform;        ///< a tab of uniform location id set by the method 'UseUniformLocation()'
                    std::vector<unsigned int>   _tabAttrib;         ///< a tab of attrib location id set by the method 'UseAttribLocation()'
            };
        }
    }
}

#endif
