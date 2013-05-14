
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

#ifndef NC_GRAPHICS_GL_GEOMETRYBUFFER_H_
#define NC_GRAPHICS_GL_GEOMETRYBUFFER_H_

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Interface defining a geometry
            /**
                Store a primitive type describing the type of primitive stored in the geometry buffer.
                \todo Use a VertexArray to optimise the rendering.
            */
            class LIB_NC_GRAPHICS IGeometryBuffer
            {
                public:
                    IGeometryBuffer(Enum::PrimitiveType primitiveType)
                        : _primitiveType(primitiveType)                         {}
                    virtual ~IGeometryBuffer()                                  {}

                    virtual IGeometryBuffer         *Clone() const = 0;

                    /** Set the primitive type to render the geometry buffer. */
                    inline void                     PrimitiveType(Enum::PrimitiveType primitiveType)         {_primitiveType = primitiveType;}

                    /** Render the geoemtry. */
                    virtual void                    Render() = 0;

                    /** \return the vertex descriptor of the VBO. */
                    virtual GL::VertexDescriptor    &Descriptor() = 0;

                    /** Map the vertex buffer in memory with the given access enum. */
                    virtual void                    MapVertexBuffer(Enum::DataBuffer::AccessType access) = 0;
                    /** Unmap the vertex buffer. */
                    virtual void                    UnmapVertexBuffer() = 0;

                    /**
                        Fetch the components in a buffer, return it and the number of elements.
                        You need to Map the buffer before calling this method.
                    */
                    template<typename VertexType>
                    VertexType                      *FetchComponents(unsigned int &size);
                    /**
                        Fetch the component with the componant name and fill the given array.
                        You need to Map the buffer before calling this method.
                    */
                    template<typename T>
                    void                            FetchComponent(const std::string &componentName, Array<T> &coords);

                protected:
                    /**
                        Fetch the components in a buffer, return it and the number of elements.
                        You need to Map the buffer before calling this method.
                    */
                    virtual void                    *FetchComponents(unsigned int &size) = 0;

                    Enum::PrimitiveType     _primitiveType;     ///< the primitive type to draw.
            };

            /// template class used to define a geometry with a VertexBuffer and an IndexBuffer.
            /**
                The template parameter is used to setup the type of vertex contained by the VertexBuffer.
                It also allows to retraive the Descriptor of the vertex type.
            */
            template<typename T, bool IndexToRender = true>
            class GeometryBuffer : public IGeometryBuffer
            {
                public:
                    GeometryBuffer(Enum::PrimitiveType type = Enum::Triangles)
                        : IGeometryBuffer(type), _needUpdate(true) {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, const IndexBuffer &ibo, Enum::PrimitiveType type)
                        : IGeometryBuffer(type), _needUpdate(true), _VBO(vbo), _IBO(ibo)    {}
                    template<unsigned int D1, unsigned int D2>
                    GeometryBuffer(const Array<T,D1> &tabVertices, GL::Enum::DataBuffer::Usage usage, const Array<unsigned int,D2> &tabIndices, unsigned int stride, Enum::PrimitiveType type)
                        : IGeometryBuffer(type), _needUpdate(true), _VBO(tabVertices, usage), _IBO(tabIndices, stride)    {}
                    virtual ~GeometryBuffer()   {}

                    virtual IGeometryBuffer         *Clone() const                          {return new GeometryBuffer<T,IndexToRender>(*this);}

                    /** \return the vertex buffer. */
                    inline VertexBuffer<T>          &VBO()                                  {return _VBO;}
                    /** Set the geometry (vertex buffer). */
                    inline void                     VBO(const VertexBuffer<T> &vbo)         {_VBO = vbo;}

                    /** \return the Index buffer of the geometry. */
                    inline IndexBuffer              &IBO()                                  {return _IBO;}
                    /** \return the Index buffer of the geometry. */
                    inline const IndexBuffer        &IBO() const                            {return _IBO;}
                    /** Set the geometry. */
                    inline void                     SetGeometry(const VertexBuffer<T> &vbo, const IndexBuffer &ibo) {_needUpdate = true; _VBO = vbo; _IBO = ibo;}

                    /** \return the vertex descriptor of the VBO. */
                    virtual GL::VertexDescriptor    &Descriptor()                           {return _VBO.Descriptor;}

                    /** Render the geometry with the index buffer. */
                    virtual void                    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        //_VAO.Enable();
                        _IBO.Draw(_primitiveType);
                        //_VAO.Disable();
                    }

                    /** Map buffer of the VBO. */
                    virtual void                    MapVertexBuffer(Enum::DataBuffer::AccessType access)
                    {
                        _VBO.Enable();
                        _VBO.MapBuffer(access);
                    }

                    /** Unmap buffer of the VBO. */
                    virtual void                    UnmapVertexBuffer()
                    {
                        _VBO.UnmapBuffer();
                        _VBO.Disable();
                    }

                private:
                    /**
                        Map the Vertex and Index buffer in the VAO (Vertex Array Object).

                        (it appears that the vertexArrays are not shared beetween contexts like the displayLists).
                        So the Map method is private and called once by the render method.
                    */
                    void    Map()
                    {
                        //_VAO = VertexArray();
                        //_VAO.Enable();
                        _VBO.Enable();
                        _VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        _VBO.Disable();
                        _IBO.Enable();
                        //_VAO.Disable();
                        _needUpdate = false;
                    }

                    /** \return the componants of the buffer and it's size. */
                    virtual void    *FetchComponents(unsigned int &size)
                    {
                        size = _VBO.Size();
                        return _VBO.LocalBuffer();
                    }

                    bool                _needUpdate;        ///< to map the vertexArray in the rendering thread.
                    //VertexArray         _VAO;               ///< the VAO used to optimised the rendering.
                    VertexBuffer<T>     _VBO;               ///< the VBO used to stored the geometry data.
                    IndexBuffer         _IBO;               ///< the index buffer.
            };

            /// Specialisation of the geometry buffer without IndexBuffer.
            template<typename T>
            class GeometryBuffer<T, false> : public IGeometryBuffer
            {
                public:
                    GeometryBuffer(Enum::PrimitiveType type = Enum::Triangles)
                        : IGeometryBuffer(type), _needUpdate(true) {}
                    GeometryBuffer(const VertexBuffer<T> &vbo, Enum::PrimitiveType type)
                        : IGeometryBuffer(type), _needUpdate(true), _VBO(vbo)   {}
                    template<unsigned int D1>
                    GeometryBuffer(const Array<T,D1> &tabVertices, GL::Enum::DataBuffer::Usage usage, Enum::PrimitiveType type)
                        : IGeometryBuffer(type), _needUpdate(true), _VBO(tabVertices, usage)    {}
                    virtual ~GeometryBuffer()   {}

                    virtual IGeometryBuffer         *Clone() const                          {return new GeometryBuffer<T,false>(*this);}

                    /** \return the vertex buffer. */
                    inline VertexBuffer<T>          &VBO()                                  {return _VBO;}
                    /** Set the geometry (vertex buffer). */
                    inline void                     VBO(const VertexBuffer<T> &vbo)         {_VBO = vbo;}

                    /** Render the geometry without index buffer. */
                    virtual void                    Render()
                    {
                        //if (_needUpdate || _VBO.NeedUpdate())
                            Map();
                        //_VAO.Enable();
                        _VBO.Draw(IGeometryBuffer::_primitiveType);
                        //_VAO.Disable();
                    }

                    /** \return the vertex descriptor of the VBO. */
                    virtual GL::VertexDescriptor    &Descriptor()                           {return _VBO.Descriptor;}

                    /** Map buffer of the VBO. */
                    virtual void                    MapVertexBuffer(Enum::DataBuffer::AccessType access)
                    {
                        _VBO.Enable();
                        _VBO.MapBuffer(access);
                    }

                    /** Unmap buffer of the VBO. */
                    virtual void                    UnmapVertexBuffer()
                    {
                        _VBO.UnmapBuffer();
                        _VBO.Disable();
                    }

                private:
                    /**
                        Map the Vertex buffer in the VAO (Vertex Array Object).

                        (it appears that the vertexArrays are not shared beetween contexts like the displayLists).
                        So the Map method is private and called once by the render method.
                    */
                    void    Map()
                    {
                        //_VAO.Enable();
                        _VBO.Enable();
                        _VBO.Map();         // no need to Unmap, we are using opengl3.x (only rendering with shader)
                        _VBO.Disable();
                        //_VAO.Disable();
                        _needUpdate = false;
                    }

                    /** \return the componants of the buffer and it's size. */
                    virtual void    *FetchComponents(unsigned int &size)
                    {
                        size = _VBO.Size();
                        return _VBO.LocalBuffer();
                    }

                    bool                _needUpdate;        ///< to map the vertexArray in the rendering thread.
                    //VertexArray         _VAO;               ///< the VAO used to optimised the rendering.
                    VertexBuffer<T>     _VBO;               ///< the VBO used to stored the geometry data.
            };


            template<typename VertexType>
            VertexType      *IGeometryBuffer::FetchComponents(unsigned int &size)
            {
                GeometryBuffer<VertexType> *geom = dynamic_cast<GeometryBuffer<VertexType>*>(this);
                if (geom != NULL)
                {
                    size = geom->VBO().Size();
                    return geom->VBO().LocalBuffer();
                }
                size = 0;
                return NULL;
            }

            template<typename T>
            void     IGeometryBuffer::FetchComponent(const std::string &componentName, Array<T> &coords)
            {
                VertexDescriptor &decript = Descriptor();
                for (unsigned int i = 0; i < decript.Size(); ++i)
                {
                    if (decript[i].Name == componentName)
                    {
                        unsigned int reelSize = decript.Sizeof();
                        if (decript[i].Sizeof == sizeof(T))
                        {
                            unsigned int size;
                            unsigned int offset = decript[i].PointerOffset;
                            unsigned int componentSize = decript[i].Size;
                            void *data = FetchComponents(size);
                            if (data != NULL)
                            {
                                coords.InitSize(componentSize * size);
                                for (unsigned int j = 0; j < size; ++j)
                                {
                                    for (unsigned int k = 0; k < componentSize; ++k)
                                    {
                                        coords.data[(j*componentSize)+k] = ((T*)((char*)data + offset + (reelSize * j)))[k];
                                    }
                                }
                            }
                        }
                       return;
                    }
                }
            }

        }
    }
}

#endif
