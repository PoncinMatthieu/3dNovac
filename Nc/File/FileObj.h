
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

    File Created At:        18/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                Implementation de la classe "gVertexBuffer"
    scpécifie un tampon de tableau de gVertex directement dans la memoire
  de la carte graphique permettant un affichage rapide et simplifié de models 3D

"gVertex" est une structure de sommet ou est enregistré :
        - sa position,
        - sa couleur,
        - ses coordonnées de texture et de normale


-----------------------------------------------------------------------------*/

#ifndef NC_FILE_OBJ_H_
#define NC_FILE_OBJ_H_

#include <list>
#include <Nc/Graphics/3D/Material/LightingMaterial.h>
#include "Define.h"

namespace Nc
{
    class LFILE FileObj
    {
        public:
            static Graphic::Object3d* Load(const Utils::FileName& aFileName, Color color = Color(255, 255, 255), unsigned int aFlags = GL_STREAM_DRAW);
            static void Save(Graphic::Object3d& aObjectToSave, Utils::FileName& aFileName); // TODO

        private:
            static bool Read(const Utils::FileName& aFileName, GL::LightingVertex** aTabVertices, unsigned int** aTabIndices, unsigned int* aNbVertices, GL::Texture &tex, GL::Texture &normalMap);
            static void GetNbsAndMtlib( std::ifstream& aFile, Utils::FileName& aMtlib);
            static Utils::FileName ReadMtl(Utils::FileName& aFile);
            static bool AllocDynamiqueTabsInterm();

            static void GetTabCoords3D(std::ifstream& aFile, Vector3f* TabCoord, unsigned int NbCoord, const std::string& ChaineDepart);
            static void GetTabCoords2D(std::ifstream& aFile, Vector2f* TabCoord, unsigned int NbCoord, const std::string& ChaineDepart);
            static void GetTabIndices(std::ifstream& aFile, unsigned int** TabIndices, unsigned int NbFaces, unsigned int Stride);
            static std::list<GL::LightingVertex> CreateListVertex(unsigned int** aTabIndices);
            static GL::LightingVertex CreateVertexIntermediaire(unsigned int i, unsigned int j);

            static Vector3f*       _tabCoordVertices;
            static Vector3f*       _tabCoordNorms;
            static Vector2f*       _tabCoordTexs;
            static unsigned int**   _tabIndices;
            static unsigned int     _nbCoordVertices;
            static unsigned int     _nbCoordTextures;
            static unsigned int     _nbCoordNormals;
            static unsigned int     _size;
            static unsigned int     _stride;
            static Color        _color;
    };
}

#endif // UFILEOBJ_H_INCLUDED
