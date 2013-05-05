
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

    File Created At:        23/07/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_UNIFORM_H_
#define NC_GRAPHICS_GL_UNIFORM_H_

#include "Program.h"
#include "Texture.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Define an interface to manage uniforms of a shader program.
            class LIB_NC_GRAPHICS IUniform
            {
                public:
                    IUniform(unsigned int location);

                    /** Set the value to the current shader program. */
                    virtual void			Set() = 0;

					/** \return the location of the uniform. */
					inline unsigned int		GetLocation()				{return _location;}

                    friend LIB_NC_GRAPHICS std::ostream &operator << (std::ostream &oss, const IUniform &u)
                    {
                        oss << u._location;
                        return oss;
                    }

                public:
                    bool            activated;

                protected:
                    unsigned int    _location;      ///< the location of the uniform into the program.
            };

            template<typename T, unsigned int Nb>
            class Uniform   {};

            template<>
            class LIB_NC_GRAPHICS Uniform<TMatrix, 1> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location), transpose(true)       {}

                    inline Uniform &operator = (const TMatrix &m)   {matrix = m; return *this;}

                    /** Set the value to the current shader program. */
                    virtual void Set()                              {if (_location != -1) Program::SetUniform(_location, matrix, transpose);}

                    TMatrix     matrix;
                    bool        transpose;
            };

            template<>
            class LIB_NC_GRAPHICS Uniform<Texture, 1> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location), unit(0)				{}

                    inline Uniform &operator = (const Texture &t)   {texture = t; return *this;}
                    inline Uniform &operator = (const int &u)       {unit = u; return *this;}

					/** Init the unit and texture value of the uniform. */
					inline void		Init(int unitValue, const Texture &textureValue)	{unit = unitValue; texture = textureValue;}

                    /** Set the value to the current shader program. */
                    virtual void	Set()
                    {
						if (_location != -1)
						{
							Program::SetUniform(_location, unit);
							GL::State::Instance().ActiveTexture(unit);
							texture.Enable();
						}
                    }

                    int         unit;
                    Texture     texture;
            };

            template<typename T>
            class Uniform<T, 1> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location)                        {}

                    inline Uniform &operator = (const T &v)         {value = v; return *this;}

                    /** Set the value to the current shader program. */
                    virtual void Set()                              {if (_location != -1) Program::SetUniform(_location, value);}

                    T       value;
            };

            template<typename T>
            class Uniform<T, 2> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location)                            {}

                    inline Uniform &operator = (const Vector2D<T> &v)   {value1 = v.data[0]; value2 = v.data[1]; return *this;}

                    /** Set the value to the current shader program. */
                    virtual void Set()                                  {if (_location != -1) Program::SetUniform(_location, value1, value2);}

                    T       value1;
                    T       value2;
            };

            template<typename T>
            class Uniform<T, 3> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location)                            {}

                    inline Uniform &operator = (const Vector3D<T> &v)   {value1 = v.data[0]; value2 = v.data[1]; value3 = v.data[2]; return *this;}

                    /** Set the value to the current shader program. */
                    virtual void Set()                                  {if (_location != -1) Program::SetUniform(_location, value1, value2, value3);}

                    T       value1;
                    T       value2;
                    T       value3;
            };

            template<typename T>
            class Uniform<T, 4> : public IUniform
            {
                public:
                    Uniform(unsigned int location)
                        : IUniform(location)                        {}

                    inline Uniform &operator = (const Color &c)     {value1 = c.r; value2 = c.g; value3 = c.b; value4 = c.a; return *this;}

                    /** Set the value to the current shader program. */
                    virtual void Set()                              {if (_location != -1) Program::SetUniform(_location, value1, value2, value3, value4);}

                    T       value1;
                    T       value2;
                    T       value3;
                    T       value4;
            };


			template class Uniform<float, 1>;
			template class Uniform<float, 2>;
		}
    }
}

#endif
