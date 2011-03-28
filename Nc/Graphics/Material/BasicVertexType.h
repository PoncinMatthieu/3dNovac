
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

    define the basic types of vertex used by the Nc::Graphics API

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_BASICVERTEXTYPE_H_
#define NC_GRAPHIC_BASICVERTEXTYPE_H_

#include "../Define.h"
#include "../Core/GL/VertexBuffer.h"

namespace Nc
{
    namespace Graphic
    {
        namespace BasicVertexType
        {
            #pragma pack(push, 1) // pragma pack to remove padding byte on structure
            struct  Colored
            {
                float           coord[3];     // position
                float           color[3];     // couleur

                static const GL::VertexDescriptor   GetDescriptor();
                void Fill(float coordX, float coordY, float coordZ, const Color &c);
            };

            struct  Colored2d
            {
                float           coord[2];     // position
                float           color[3];     // couleur

                static const GL::VertexDescriptor   GetDescriptor();
                void Fill(float coordX, float coordY, const Color &c);
            };

            struct  Textured
            {
                float           coord[3];     // position
                float           texCoord[2];   // coord de texture
                float           color[3];     // couleur
                float           normal[3];

                static const GL::VertexDescriptor   &GetDescriptor();
                void Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float normalX, float normalY, float normalZ, const Color &c);
            };

            struct  Textured2d
            {
                float           coord[2];     // position
                float           texCoord[2];   // coord de texture
                float           color[3];     // couleur

                static const GL::VertexDescriptor   &GetDescriptor();
                void Fill(float coordX, float coordY, float texCoordX, float texCoordY, const Color &c);
            };

            struct  Textured3d
            {
                float           coord[3];     // position
                float           texCoord[3];  // coord de texture
                float           color[3];     // couleur

                static const GL::VertexDescriptor   &GetDescriptor();
                void Fill(float coordX, float coordY, float coordZ, float texCoordX, float texCoordY, float texCoordZ, const Color &c);
            };
            #pragma pack(pop)
        }
    }
}

#endif
