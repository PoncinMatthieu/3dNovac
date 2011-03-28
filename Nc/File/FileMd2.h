
/*-----------------------------------------------------------------------------

	3dNovac File
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

    File Created At:        30/04/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                     Implementation de l'interface "uFile3ds"
                        permettant de loader un fichier 3ds

-----------------------------------------------------------------------------*/

#ifndef NOVAC_FILEMD2_H_INCLUDED
#define NOVAC_FILEMD2_H_INCLUDED

#include <list>
#include <map>

#include "Define.h"

namespace Nc
{
    class LFILE FileMd2
    {
        public:
            static Graphic::Object3d* Load(const Utils::FileName& aFileName, unsigned int aFlags = GL_STREAM_DRAW);

        private:


            // header md2
            typedef struct
            {
                int ident;          // numéro magique : "IDP2"
                int version;        // version du format : 8

                int skinwidth;      // largeur texture
                int skinheight;     // hauteur texture

                int framesize;      // taille d'une frame en octets

                int num_skins;      // nombre de skins
                int num_vertices;   // nombre de vertices par frame
                int num_st;         // nombre de coordonnées de texture
                int num_tris;       // nombre de triangles
                int num_glcmds;     // nombre de commandes opengl
                int num_frames;     // nombre de frames

                int offset_skins;   // offset données skins
                int offset_st;      // offset données coordonnées de texture
                int offset_tris;    // offset données triangles
                int offset_frames;  // offset données frames
                int offset_glcmds;  // offset données commandes OpenGL
                int offset_end;     // offset fin de fichier
            } t_md2_header;
    };
}

#endif // UFILEOBJ_H_INCLUDED

