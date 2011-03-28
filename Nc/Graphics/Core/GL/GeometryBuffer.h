
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

    File Created At:        04/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Implemente a class to define a geometry with a VBO/IBO/VAO

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_GEOMETRYBUFFER_H_
#define NC_GRAPHICS_CORE_GL_GEOMETRYBUFFER_H_

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            class LGRAPHICS IGeometryBuffer
            {
                public:
                    IGeometryBuffer()               {}
                    virtual ~IGeometryBuffer()      {}

                    virtual void    Render() = 0;
            };

            template<typename T, bool IndexToRender = true>
            class GeometryBuffer : public IGeometryBuffer
            {
                public:
                    GeometryBuffer() : _needUpdate(true), _primitiveType(GL_TRIANGLES) {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, const IndexBuffer &ibo, GLenum primitiveType) : _needUpdate(true), _VBO(vbo), _IBO(ibo), _primitiveType(primitiveType) {}
                    virtual ~GeometryBuffer()   {}

                    inline VertexBuffer<T>          &GetVBO()                                   {return _VBO;}
                    inline IndexBuffer              &GetIBO()                                   {return _IBO;}
                    inline void                     SetPrimitiveType(GLenum primitiveType)      {_primitiveType = primitiveType;}
                    inline void                     SetGeometry(const VertexBuffer<T> &vbo, const IndexBuffer &ibo) {_needUpdate = true; _VBO = vbo; _IBO = ibo;}

                    void    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        _VAO.Enable();
                        _IBO.Draw(_primitiveType);
                        _VAO.Disable();
                    }

                private:
                    // (it appears that the vertexArrays are not shared beetween contexts like the displayLists)
                    // so the Map method is private and called once by the render method
                    void    Map()
                    {
                        _VAO = VertexArray();
                        _VAO.Enable();
                        _VBO.Enable();
                        _VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        _VBO.Disable();
                        _IBO.Enable();
                        _VAO.Disable();
                        _needUpdate = false;
                    }

                    bool                _needUpdate;    // to map the vertexArray in the rendering thread
                    VertexBuffer<T>     _VBO;
                    IndexBuffer         _IBO;
                    VertexArray         _VAO;
                    unsigned int        _primitiveType;  // GL_TRIANGLES, GL_QUADS, etc..
            };

            // specialisation, if we don't use the IBO
            template<typename T>
            class GeometryBuffer<T, false> : public IGeometryBuffer
            {
                public:
                    GeometryBuffer() : _needUpdate(true), _primitiveType(GL_TRIANGLES) {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, GLenum primitiveType) : _needUpdate(true), _VBO(vbo), _primitiveType(primitiveType) {}
                    virtual ~GeometryBuffer()   {}

                    inline VertexBuffer<T>          &GetVBO()                                   {return _VBO;}
                    inline void                     SetPrimitiveType(GLenum primitiveType)      {_primitiveType = primitiveType;}
                    inline void                     SetGeometry(const VertexBuffer<T> &vbo)     {_VBO = vbo;}

                    void    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        _VAO.Enable();
                        _VBO.Draw(_primitiveType);
                        _VAO.Disable();
                    }

                private:
                    // (it appears that the vertexArrays are not shared beetween contexts like the displayLists)
                    // so the Map method is private and called once by the render method
                    void    Map()
                    {
                        _VAO.Enable();
                        _VBO.Enable();
                        _VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        _VBO.Disable();
                        _VAO.Disable();
                        _needUpdate = false;
                    }

                    bool                _needUpdate;    // to map the vertexArray in the rendering thread
                    VertexBuffer<T>     _VBO;
                    VertexArray         _VAO;
                    unsigned int        _primitiveType;  // GL_TRIANGLES, GL_QUADS, etc..
            };
        }
    }
}

#endif
