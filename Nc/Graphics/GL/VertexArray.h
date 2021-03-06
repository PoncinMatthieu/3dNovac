
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

    File Created At:        04/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_VERTEXARRAY_H_
#define NC_GRAPHICS_GL_VERTEXARRAY_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// To use an Vertex Array Object (VAO)
            /**
                \todo VertexArray doesn't work properly and has been disabled into the GeometryBuffer class.
             */
            class LIB_NC_GRAPHICS VertexArray : public Object
            {
                public:
                    VertexArray();
                    virtual ~VertexArray();

                    /** Create a new copy of the object by without duplicate the ogl resources. */
                    virtual Object          *Clone() const      {return new VertexArray(*this);}

                    /** Return the index of the VAO. */
                    virtual unsigned int    GetIndex() const    {return _index;}
                    /** Enable the VAO. */
                    virtual void            Enable() const;
                    /** Disable the VAO. */
                    virtual void            Disable() const;

                private:
                    /** Release the VAO. */
                    virtual void            Release();

                    unsigned int    _index;         ///< the index of the VAO.
            };
        }
    }
}

#endif
