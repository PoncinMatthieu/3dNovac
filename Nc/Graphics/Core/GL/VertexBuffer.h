
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

    File Created At:        18/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_GL_VERTEXBUFFER_H_
#define NC_GRAPHICS_CORE_GL_VERTEXBUFFER_H_

#include <iostream>
#include <list>

#include "../../Define.h"
#include "IndexBuffer.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Structure to define a data of a VertexDescriptor that describe a composant of a VertexType
            struct DataVertexDescriptor
            {
                int             Size;               ///< the size of the data in the vertex
                GLenum          Type;               ///< the type of the data in the vertex
                unsigned int    PointerOffset;      ///< the pointer offset of the data in the vertex
                unsigned int    IndexAttrib;        ///< the index Attrib (in relation with the Shader::AttribLocation) of the data in the vertex
                bool            Normalized;         ///< if true, the data will be normalized before enterring in the shader program pipeline
            };
            typedef Array<DataVertexDescriptor>     VertexDescriptor;   ///< define the componants of a VertexType

            /// Interface to define a VertexBuffer
            class LGRAPHICS IVertexBuffer
            {
                public:
                    IVertexBuffer(const VertexDescriptor &descriptor) : Descriptor(descriptor), MaskDescriptor(0xffff)    {}
                    virtual ~IVertexBuffer()    {}

                    /** Return true if we need to update the geometry */
                    inline bool         NeedUpdate()                        {return _needUpdate;}
                    /** Map the buffer (activate it to be rendered) */
                    virtual void        Map() = 0;
                    /** Unmap the buffer (Disable it) */
                    virtual void        Unmap() = 0;
                    /**
                        Draw the vertex buffer with the given primitive type <br/>
                        Primitive type could be :       <br/>
                            GL_POINTS,                  <br/>
                            GL_LINE_STRIP,              <br/>
                            GL_LINE_LOOP,               <br/>
                            GL_LINES,                   <br/>
                            GL_LINE_STRIP_ADJACENCY,    <br/>
                            GL_LINES_ADJACENCY,         <br/>
                            GL_TRIANGLE_STRIP,          <br/>
                            GL_TRIANGLE_FAN,            <br/>
                            GL_TRIANGLES,               <br/>
                            GL_TRIANGLE_STRIP_ADJACENCY and <br/>
                            GL_TRIANGLES_ADJACENCY      <br/>
                     */
                    virtual void        Draw(GLenum primitiveType) = 0;

                    VertexDescriptor    Descriptor;         ///< used to describe the vertex type structure
                    unsigned short      MaskDescriptor;     ///< used to render the good componants vertex

                protected:
                    bool                _needUpdate;        ///< used to mark if we need to update the geometry
            };

            /// Template class which define a buffer of vertex to describe a geometry
            template<typename T>
            class VertexBuffer : public IVertexBuffer, public DataBuffer<T>
            {
                public:
                    VertexBuffer();
                    VertexBuffer(unsigned int flags);
                    template<unsigned int D>
                    VertexBuffer(const Array<T,D> &tabVertices, unsigned int flags, bool keepContent = false);
                    virtual ~VertexBuffer();

                    // manage content
                    /** Init the buffer */
                    void    Init();
                    /** Init the buffer */
                    void    Init(unsigned int size, unsigned int flags);
                    /** Update the buffer */
                    template<unsigned int D>
                    void    UpdateData(const Array<T,D> &tabVertices, unsigned int flags, bool keepContent = false);
                    /** Update the buffer */
                    void    UpdateData(const T *aDataTab, bool conserveData = false);

                    // affichage
                    /** Map the buffer (Activate it to be rendered) */
                    virtual void        Map();
                    /** Unmap the buffer (Disable it) */
                    virtual void        Unmap();
                    /** Draw the buffer with the given primitive type */
                    virtual void        Draw(GLenum primitiveType);
            };

            template<typename T>
            VertexBuffer<T>::VertexBuffer()
                : IVertexBuffer(T::GetDescriptor()), DataBuffer<T>()
            {
            }

            template<typename T>
            template<unsigned int D>
            VertexBuffer<T>::VertexBuffer(const Array<T,D> &tabVertices, unsigned int flags, bool keepContent)
                : IVertexBuffer(T::GetDescriptor()), DataBuffer<T>(GL_ARRAY_BUFFER, tabVertices.Size(), 0, flags, (T*)tabVertices.Data, keepContent)
            {
                _needUpdate = true;
            }

            template<typename T>
            VertexBuffer<T>::~VertexBuffer()
            {
            }

            template<typename T>
            void VertexBuffer<T>::Init()
            {
                DataBuffer<T>::Init(GL_ARRAY_BUFFER);
                _needUpdate = true;
            }

            template<typename T>
            void VertexBuffer<T>::Init(unsigned int size, unsigned int flags)
            {
                DataBuffer<T>::Init(GL_ARRAY_BUFFER, size, 0, flags, (T*)NULL, false);
                _needUpdate = true;
            }

            template<typename T>
            template<unsigned int D>
            void VertexBuffer<T>::UpdateData(const Array<T,D> &tabVertices, unsigned int flags, bool keepContent)
            {
                if (!DataBuffer<T>::IsValid())
                    DataBuffer<T>::Init(GL_ARRAY_BUFFER, tabVertices.Size(), 0, flags, (T*)tabVertices.Data, keepContent);
                else
                {
                    DataBuffer<T>::Enable();
                    if (tabVertices.Size() == DataBuffer<T>::_size)
                        DataBuffer<T>::UpdateData((T*)tabVertices.Data, keepContent);
                    else
                        DataBuffer<T>::UpdateData(tabVertices.Size(), 0, flags, (T*)tabVertices.Data, keepContent);
                    DataBuffer<T>::Disable();
                }
                _needUpdate = true;
            }

            template<typename T>
            void    VertexBuffer<T>::UpdateData(const T *aDataTab, bool conserveData)
            {
                DataBuffer<T>::Enable();
                DataBuffer<T>::UpdateData(aDataTab, conserveData);
                DataBuffer<T>::Disable();
                _needUpdate = true;
            }

            template<typename T>
            void    VertexBuffer<T>::Map()
            {
                #define BUFFER_OFFSET(n) ((char*)NULL + (n))

                // enable client states and set pointer, Draw, and then disable the client states
                for (unsigned short i = 0; i < Descriptor.Size(); ++i)
                {
                    if ((MaskDescriptor >> i) & 1)
                    {
                        DataVertexDescriptor &desc = Descriptor[i];
                        //if (desc.IndexAttrib >= 0)
                        //{
                            glEnableVertexAttribArray(desc.IndexAttrib);
                            glVertexAttribPointer(desc.IndexAttrib, desc.Size, desc.Type, desc.Normalized, sizeof(T), BUFFER_OFFSET(desc.PointerOffset));
                        //}
                    }
                }
                _needUpdate = false;

                #undef BUFFER_OFFSET
            }

            template<typename T>
            void    VertexBuffer<T>::Unmap()
            {
                for (unsigned short i = 0; i < Descriptor.Size(); ++i)
                {
                    if ((MaskDescriptor >> i) | 1)
                    {
                        DataVertexDescriptor &desc = Descriptor[i];
                        //if (desc.IndexAttrib >= 0)
                            glDisableVertexAttribArray(desc.IndexAttrib);
                    }
                }
            }

            template<typename T>
            void    VertexBuffer<T>::Draw(GLenum primitiveType)
            {
                glDrawArrays(primitiveType, 0, DataBuffer<T>::_size);
            }
        }
    }
}

#endif
