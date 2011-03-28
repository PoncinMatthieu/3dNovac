
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

    File Created At:        22/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                  Implementation de la classe mere "Material"

        Classe de base permettant de definir un type de materiaux,
                    chaque mesh est associer a un materiaux,
                celui ci, s'occupera de faire le rendu du mesh
    en fonction du type de materiaux et des parametres qui lui sont fourni

-----------------------------------------------------------------------------*/


#ifndef NC_GRAPHIC_MATERIAL_MATERIAL_H_
#define NC_GRAPHIC_MATERIAL_MATERIAL_H_

#include "DefaultMaterials.h"

namespace Nc
{
    namespace Graphic
    {
        template<typename VertexType, typename MaterialPolitic>
        class Material : public MaterialPolitic, public Utils::Singleton<Material<VertexType, MaterialPolitic> >
        {
            public:
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, GL::GeometryBuffer<VertexType, INDEX> &geometry, MaterialConfig<VertexType, ConfigPolitic> &config)
                {
                    MaterialPolitic::Render(scene, scene->ModelMatrix(), geometry, config);
                }

                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, GL::GeometryBuffer<VertexType, INDEX> &geometry, MaterialConfig<VertexType, ConfigPolitic> &config)
                {
                    MaterialPolitic::Render(scene, modelMatrix, geometry, config);
                }
        };
    }
}

#endif
