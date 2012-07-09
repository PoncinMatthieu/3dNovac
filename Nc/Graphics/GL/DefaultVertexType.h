
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

#ifndef NC_GRAPHICS_GL_DEFAULTVERTEXTYPE_H_
#define NC_GRAPHICS_GL_DEFAULTVERTEXTYPE_H_

#include "../Define.h"
#include "VertexBuffer.h"

namespace Nc
{
    namespace Graphic
    {
        namespace GL
        {
            /// Reference some struct that define default VertexTypes.
            namespace DefaultVertexType
            {
                /// Define the names of the defaults componants which should used in the default shaders.
                namespace ComponentsName
                {
                    static const std::string Coord = "Coord";
                    static const std::string TexCoord = "TexCoord";
                    static const std::string Color = "Color";
                    static const std::string Normal = "Normal";
                }

                #pragma pack(push, 1) // pragma pack to remove padding byte on structure
                /// A VertexType for 3d colored geometry.
                struct LGRAPHICS Colored
                {
                    float           coord[3];     // position
                    float           color[3];     // color

                    static const GL::VertexDescriptor   &GetDescriptor();
                    static unsigned int                 GetDescriptorPriority()             {return sizeof(Colored);}
                    void Fill(float coordX, float coordY, float coordZ, const Color &c);
                };

                /// A VertexType for 2d colored geometry.
                struct LGRAPHICS Colored2d
                {
                    float           coord[2];     // position
                    float           color[3];     // color

                    static const GL::VertexDescriptor   &GetDescriptor();
                    static unsigned int                 GetDescriptorPriority()             {return sizeof(Colored2d);}
                    void Fill(float coordX, float coordY, const Color &c);
                };

                /// A VertexType for 3d textured geometry (with a normal to apply lighting on it).
                struct LGRAPHICS Textured
                {
                    float           coord[3];     // position
                    float           texCoord[2];   // texture coord
                    float           color[3];     // color
                    float           normal[3];

                    static const GL::VertexDescriptor   &GetDescriptor();
                    static unsigned int                 GetDescriptorPriority()             {return sizeof(Textured);}
                    void Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, const Color &c);
                };

                /// A VertexType for 2d textured geometry.
                struct LGRAPHICS Textured2d
                {
                    float           coord[2];     // position
                    float           texCoord[2];   // texture coord
                    float           color[3];     // color

                    static const GL::VertexDescriptor   &GetDescriptor();
                    static unsigned int                 GetDescriptorPriority()             {return sizeof(Textured2d);}
                    void Fill(float coordX, float coordY, float texCoordX, float texCoordY, const Color &c);
                };

                /// A VertexType for 3d textured geometry (a SkyBox use these VertexType).
                struct LGRAPHICS Textured3d
                {
                    float           coord[3];     // position
                    float           texCoord[3];  // texture coord
                    float           color[3];     // color

                    static const GL::VertexDescriptor   &GetDescriptor();
                    static unsigned int                 GetDescriptorPriority()             {return sizeof(Textured3d);}
                    void Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float texCoordZ, const Color &c);
                };
                #pragma pack(pop)
            }
        }
    }
}

#endif
