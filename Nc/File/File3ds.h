
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

    File Created At:        02/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                     Implementation de l'interface "uFile3ds"
                        permettant de loader un fichier 3ds

    /// TODO : Update to lib3ds v2.0

-----------------------------------------------------------------------------*/

#ifndef NOVAC_FILE3DS_H_INCLUDED
#define NOVAC_FILE3DS_H_INCLUDED

#include <list>
#include <map>
#include <lib3ds/file.h>

//#include <Nc/Graphics/Material/LightingMaterial.h>
#include "Define.h"

namespace Nc
{
    class File3ds
    {
        private:
            typedef struct                      s_material
            {
                std::list<unsigned int>         listIndice;
                std::list<GL::LightingVertex*>  listVertex;
                GL::Texture                     texture;
                GL::Texture                     normalMap;
                Box3f                           box;
            }                                   t_material;

        public:
            static Graphic::Object3d* Load(const Utils::FileName& aFileName, unsigned int aFlags = GL_STREAM_DRAW);

        private:
            static void CreateMeshs(Lib3dsMesh *mesh, unsigned int flags);
            static void NewMesh(Lib3dsMesh *mesh, t_material *mat, unsigned int flags);

            static void FillTabVertice(Lib3dsMesh *mesh);
            static std::map<std::string, t_material*> ParseFaces(Lib3dsMesh *mesh);

            static t_material *GetMaterial(std::map<std::string, t_material*> &mapMaterial, Lib3dsFace &f);
            static GL::Texture GetTexture(Lib3dsMaterial *mat, GL::Texture &normalMap);

            static Utils::FileName              _fileName;
            static GL::LightingVertex         *_tabVertices;
            static std::map<std::string, GL::Texture>   _mapTexture;
            static Graphic::ListPMesh           _listMesh;
            static Lib3dsFile                   *_file;
            static bool                         _first;

    };
}

#endif // UFILEOBJ_H_INCLUDED
