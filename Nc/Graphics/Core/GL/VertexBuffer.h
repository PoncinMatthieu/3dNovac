
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
#include "VertexDescriptor.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Template class which define a buffer of vertex to describe a geometry
            template<typename T>
            class VertexBuffer : public DataBuffer<T>
            {
                public:
                    VertexBuffer();
                    VertexBuffer(unsigned int flags);
                    template<unsigned int D>
                    VertexBuffer(const Array<T,D> &tabVertices, unsigned int flags);
                    ~VertexBuffer();

                    // manage content
                    /** Init the buffer */
                    void    Init();
                    /** Init the buffer */
                    void    Init(unsigned int size, unsigned int flags);
                    /** Update the buffer */
                    template<unsigned int D>
                    void    UpdateData(const Array<T,D> &tabVertices, unsigned int flags);
                    /** Update the buffer */
                    void    UpdateData(const T *aDataTab);

                    /** \return true if we need to update the geometry */
                    inline bool         NeedUpdate()                        {return _needUpdate;}

                    // affichage
                    /** Map the buffer (Activate it to be rendered) */
                    void    Map();
                    /** Unmap the buffer (Disable it) */
                    void    Unmap();

                    /**
                        Draw the vertex buffer with the given primitive type <br/>
                        Primitive type could be :
                            - GL_POINTS
                            - GL_LINE_STRIP
                            - GL_LINE_LOOP
                            - GL_LINES
                            - GL_LINE_STRIP_ADJACENCY
                            - GL_LINES_ADJACENCY
                            - GL_TRIANGLE_STRIP
                            - GL_TRIANGLE_FAN
                            - GL_TRIANGLES
                            - GL_TRIANGLE_STRIP_ADJACENCY
                            - GL_TRIANGLES_ADJACENCY
                     */
                    void    Draw(GLenum primitiveType);

                public:
                    VertexDescriptor    Descriptor;         ///< used to describe the vertex type structure

                private:
                    bool    _needUpdate;        ///< used to mark if we need to update the geometry
            };

            // implementation
            template<typename T>
            VertexBuffer<T>::VertexBuffer()
                : DataBuffer<T>(), Descriptor(T::GetDescriptor()), _needUpdate(true)
            {
            }

            template<typename T>
            template<unsigned int D>
            VertexBuffer<T>::VertexBuffer(const Array<T,D> &tabVertices, unsigned int flags)
                : DataBuffer<T>(GL_ARRAY_BUFFER, tabVertices.Size(), 0, flags, (T*)tabVertices.Data),
                  Descriptor(T::GetDescriptor()), _needUpdate(true)
            {
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
                DataBuffer<T>::Init(GL_ARRAY_BUFFER, size, 0, flags, (T*)NULL);
                _needUpdate = true;
            }

            template<typename T>
            template<unsigned int D>
            void VertexBuffer<T>::UpdateData(const Array<T,D> &tabVertices, unsigned int flags)
            {
                if (!DataBuffer<T>::IsValid())
                    DataBuffer<T>::Init(GL_ARRAY_BUFFER, tabVertices.Size(), 0, flags, (T*)tabVertices.Data);
                else
                {
                    DataBuffer<T>::Enable();
                    if (tabVertices.Size() == DataBuffer<T>::_size)
                        DataBuffer<T>::UpdateData((T*)tabVertices.Data);
                    else
                        DataBuffer<T>::UpdateData(tabVertices.Size(), 0, flags, (T*)tabVertices.Data);
                    DataBuffer<T>::Disable();
                }
                _needUpdate = true;
            }

            template<typename T>
            void    VertexBuffer<T>::UpdateData(const T *aDataTab)
            {
                DataBuffer<T>::Enable();
                DataBuffer<T>::UpdateData(aDataTab);
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
                    DataVertexDescriptor &desc = Descriptor[i];
                    if (desc.Bind)
                    {
                        glEnableVertexAttribArray(desc.IndexAttrib);
                        glVertexAttribPointer(desc.IndexAttrib, desc.Size, desc.Type, desc.Normalized, sizeof(T), BUFFER_OFFSET(desc.PointerOffset));
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
                    DataVertexDescriptor &desc = Descriptor[i];
                    if (desc.Bind)
                        glDisableVertexAttribArray(desc.IndexAttrib);
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
