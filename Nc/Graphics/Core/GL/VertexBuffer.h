
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
/*-----------------------------------------------------------------------------


          Implementation des classes "gVertexBuffer" et "gIndexBuffer"
 scpécifie un tampon de tableau de gVertex et d'index directement dans la memoire
  de la carte graphique permettant un affichage rapide et simplifié de models 3D

"gVertex" est une structure de sommet ou est enregistré :
        - sa position,
        - sa couleur,
        - ses coordonnées de texture et de normale

Le parametre "aFlags " prent pour valeur :
        - GL_STREAM_DRAW : vous mettez vos données à jour à chaque affichage ( glDraw*() )
        - GL_DYNAMIC_DRAW : vous mettez fréquemment à jour vos données. (plus d'une fois par affichage)
        - GL_STATIC_DRAW : vous mettez peu souvent voir jamais vos données à jour
Dans les gIndexBuffer, on utilise par defaut le "GL_STATIC_DRAW", on considere
qu'une fois créé, il ne bouge plus, ce sont plutot les coordonnées des vertex qui
peuvent changer

Les rendu se feront par la classe "VertexBuffer" en appellan la fonction "Draw"
et en lui passant en parametre un "IndexBuffer"

-----------------------------------------------------------------------------*/


#ifndef NC_GRAPHICS_CORE_GL_VERTEXBUFFER_H_
#define NC_GRAPHICS_CORE_GL_VERTEXBUFFER_H_

#include <iostream>
#include <list>

#include "../../Define.h"
#include "IndexBuffer.h"

#define NC_VBO_BUFFER_OFFSET(n) ((char*)NULL + (n))

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            struct DataVertexDescriptor
            {
                int             Size;
                GLenum          Type;
                unsigned int    PointerOffset;
                unsigned int    IndexAttrib;
                bool            Normalized;
            };
            typedef Array<DataVertexDescriptor>     VertexDescriptor;

            class LGRAPHICS IVertexBuffer
            {
                public:
                    IVertexBuffer(const VertexDescriptor &descriptor) : Descriptor(descriptor), MaskDescriptor(0xffff)    {}
                    virtual ~IVertexBuffer()    {}

                    inline bool         NeedUpdate()                        {return _needUpdate;}
                    virtual void        Map() = 0;
                    virtual void        Unmap() = 0;
                    virtual void        Draw(GLenum primitiveType) = 0;

                    VertexDescriptor    Descriptor;         ///< used to describe the vertex structure
                    unsigned short      MaskDescriptor;     ///< used to render the good data vertex

                protected:
                    bool                _needUpdate;
            };

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
                    void    Init();
                    void    Init(unsigned int size, unsigned int flags);
                    template<unsigned int D>
                    void    UpdateData(const Array<T,D> &tabVertices, unsigned int flags, bool keepContent = false);
                    void    UpdateData(const T *aDataTab, bool conserveData = false);

                    // affichage
                    virtual void        Map();
                    virtual void        Unmap();
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
                /// enable client states and set pointer, Draw, and then disable the client states
                for (unsigned short i = 0; i < Descriptor.Size(); ++i)
                {
                    if ((MaskDescriptor >> i) & 1)
                    {
                        DataVertexDescriptor &desc = Descriptor[i];
                        if (desc.IndexAttrib >= 0)
                        {
                            glEnableVertexAttribArray(desc.IndexAttrib);
                            glVertexAttribPointer(desc.IndexAttrib, desc.Size, desc.Type, desc.Normalized, sizeof(T), NC_VBO_BUFFER_OFFSET(desc.PointerOffset));
                        }
                    }
                }
                _needUpdate = false;
            }

            template<typename T>
            void    VertexBuffer<T>::Unmap()
            {
                for (unsigned short i = 0; i < Descriptor.Size(); ++i)
                {
                    if ((MaskDescriptor >> i) | 1)
                    {
                        DataVertexDescriptor &desc = Descriptor[i];
                        if (desc.IndexAttrib >= 0)
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
