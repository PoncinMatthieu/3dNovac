
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
            /// template interface wich define a geometry with it's given VertexType
            /**
                Define the geometry with a VertexBuffer and a primitive type
                And a VertexArray to optimised the rendering
            */
            template<typename T>
            class LGRAPHICS IGeometryBuffer
            {
                public:
                    IGeometryBuffer()  : _needUpdate(true), _primitiveType(GL_TRIANGLES)    {}
                    IGeometryBuffer(const VertexBuffer<T> &vbo, GLenum primitiveType)  : _needUpdate(true), _primitiveType(GL_TRIANGLES)    {}
                    virtual ~IGeometryBuffer()      {}

                    /** Return the vertex buffer */
                    inline VertexBuffer<T>          &GetVBO()                                   {return _VBO;}
                    /** Set the primitive type to render the geometry buffer */
                    inline void                     SetPrimitiveType(GLenum primitiveType)      {_primitiveType = primitiveType;}

                    /** Set the geometry (vertex buffer) */
                    inline void                     SetGeometry(const VertexBuffer<T> &vbo)     {_VBO = vbo;}

                    /** Render the geoemtry */
                    virtual void    Render() = 0;

                protected:
                    bool                _needUpdate;        ///< to map the vertexArray in the rendering thread
                    VertexBuffer<T>     _VBO;               ///< the VBO used to stored the geometry data
                    VertexArray         _VAO;               ///< the VAO used to optimised the rendering
                    unsigned int        _primitiveType;     ///< Should be: GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES, GL_LINE_STRIP_ADJACENCY, GL_LINES_ADJACENCY, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN, GL_TRIANGLES, GL_TRIANGLE_STRIP_ADJACENCY and GL_TRIANGLES_ADJACENCY
            };

            /// template class to define a geometry with an index buffer
            template<typename T, bool IndexToRender = true>
            class GeometryBuffer : public IGeometryBuffer<T>
            {
                public:
                    GeometryBuffer() {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, const IndexBuffer &ibo, GLenum primitiveType) : IGeometryBuffer<T>(vbo, primitiveType), _IBO(ibo)    {}
                    virtual ~GeometryBuffer()   {}

                    /** Return the Index buffer of the geometry */
                    inline IndexBuffer              &GetIBO()                                   {return _IBO;}
                    /** Set the geometry */
                    inline void                     SetGeometry(const VertexBuffer<T> &vbo, const IndexBuffer &ibo) {IGeometryBuffer<T>::_needUpdate = true; IGeometryBuffer<T>::_VBO = vbo; _IBO = ibo;}

                    /** Render the geometry with the index buffer */
                    void    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        IGeometryBuffer<T>::_VAO.Enable();
                        _IBO.Draw(IGeometryBuffer<T>::_primitiveType);
                        IGeometryBuffer<T>::_VAO.Disable();
                    }

                private:
                    /**
                        Map the Vertex and Index buffer in the VAO (Vertex Array Object). <br/>
                        (it appears that the vertexArrays are not shared beetween contexts like the displayLists)
                        So the Map method is private and called once by the render method
                    */
                    void    Map()
                    {
                        IGeometryBuffer<T>::_VAO = VertexArray();
                        IGeometryBuffer<T>::_VAO.Enable();
                        IGeometryBuffer<T>::_VBO.Enable();
                        IGeometryBuffer<T>::_VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        IGeometryBuffer<T>::_VBO.Disable();
                        _IBO.Enable();
                        IGeometryBuffer<T>::_VAO.Disable();
                        IGeometryBuffer<T>::_needUpdate = false;
                    }

                    IndexBuffer         _IBO;           ///< the index buffer
            };

            /// Specialisation of the geometry buffer without IndexBuffer
            template<typename T>
            class GeometryBuffer<T, false> : public IGeometryBuffer<T>
            {
                public:
                    GeometryBuffer() {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, GLenum primitiveType) : IGeometryBuffer<T>(vbo, primitiveType)   {}
                    virtual ~GeometryBuffer()   {}

                    /** Render the geometry without index buffer */
                    virtual void    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        IGeometryBuffer<T>::_VAO.Enable();
                        IGeometryBuffer<T>::_VBO.Draw(IGeometryBuffer<T>::_primitiveType);
                        IGeometryBuffer<T>::_VAO.Disable();
                    }

                private:
                    /**
                        Map the Vertex buffer in the VAO (Vertex Array Object). <br/>
                        (it appears that the vertexArrays are not shared beetween contexts like the displayLists)
                        So the Map method is private and called once by the render method
                    */
                    void    Map()
                    {
                        IGeometryBuffer<T>::_VAO.Enable();
                        IGeometryBuffer<T>::_VBO.Enable();
                        IGeometryBuffer<T>::_VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        IGeometryBuffer<T>::_VBO.Disable();
                        IGeometryBuffer<T>::_VAO.Disable();
                        IGeometryBuffer<T>::_needUpdate = false;
                    }
            };
        }
    }
}

#endif
