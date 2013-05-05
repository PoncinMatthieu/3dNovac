
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

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

    File Created At:        30/05/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_GL_VERTEXDESCRIPTOR_H_
#define NC_GRAPHICS_GL_VERTEXDESCRIPTOR_H_

#include "GLObject.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Structure to define a data of a VertexDescriptor that describe a composant of a VertexType.
            struct LIB_NC_GRAPHICS DataVertexDescriptor
            {
				DataVertexDescriptor();
				~DataVertexDescriptor();

                template<typename T>
				void Init(const std::string &name, int size, GLenum type, unsigned int pointerOffset, bool normalized = false)
				{
				    Sizeof = sizeof(T);
				    Name = name;
					Size = size;
					Type = type;
					PointerOffset = pointerOffset;
					Normalized = normalized;
				}

                friend LIB_NC_GRAPHICS std::ostream &operator << (std::ostream &oss, const DataVertexDescriptor &d)
                {
                    oss << "Size: " << d.Size << "\tType: " << d.Type << "\tPointerOffset: " << d.PointerOffset
                        << "\tIndexAttrib: " << d.IndexAttrib << "\tNormalized: " << d.Normalized;
                    return oss;
                }

                size_t          Sizeof;             ///< the size corresponding to the size of the componant type.
                std::string     Name;               ///< the name of the componant used to identify it.
                int             Size;               ///< the size of the data in the vertex.
                GLenum          Type;               ///< the type of the data in the vertex.
                unsigned int    PointerOffset;      ///< the pointer offset of the data in the vertex.
                int             IndexAttrib;        ///< the index Attrib (in relation with the Shader::AttribLocation) of the data in the vertex.
                bool            Normalized;         ///< if true, the data will be normalized before enterring in the shader program pipeline.
            };

            /// Describe all componants of a VertexType.
            class LIB_NC_GRAPHICS VertexDescriptor : public Array<DataVertexDescriptor,0>
            {
                public:
                    VertexDescriptor(const std::string &name, unsigned int Sizeof, unsigned int nbComponent, unsigned int priority);
					~VertexDescriptor();

                    inline size_t       Sizeof()        {return _sizeof;}
                    inline unsigned int Priority()      {return _priority;}

                    friend LIB_NC_GRAPHICS std::ostream &operator << (std::ostream &oss, const VertexDescriptor &d)
                    {
                        oss << "Descriptor: " << d._name << std::endl << "Priority = " << d._priority << std::endl;
                        for (unsigned int i = 0; i < d.Size(); ++i)
                            oss << d.data[i] << std::endl;
                        return oss;
                    }

                private:
                    std::string     _name;          ///< Name of the vertex type.
                    size_t          _sizeof;        ///< size of the vertex structure.
                    unsigned int    _priority;      ///< priority used to choose the best default material.
            };
        }
    }
}

#endif
