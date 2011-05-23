
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

    File Created At:        22/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_MATERIAL_MATERIAL_H_
#define NC_GRAPHIC_MATERIAL_MATERIAL_H_

#include "DefaultMaterials.h"

namespace Nc
{
    namespace Graphic
    {
        /// To render a drawable with a Politic that affect how the drawable is rendered
        /**
            A MaterialPolitic should define these 2 methodes :
 \code
    void    Configure(GL::IGeometryBuffer<VertexType> &geometry);

    void    Render(ISceneGraph *scene, TMatrix &modelMatrix, Drawable<VertexType, INDEX, ConfigPolitic> &drawable);
 \endcode
            If you are looking for an exemple of material politics you should show the DefaultMaterialPolitics and the DefaultLightingMaterialConfigPolitic classes.
        */
        template<typename VertexType, typename MaterialPolitic>
        class Material : public MaterialPolitic, public Utils::Singleton<Material<VertexType, MaterialPolitic> >
        {
            public:
                /** Render the given drawable with the scene using the Politic */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, Drawable<VertexType, INDEX, ConfigPolitic> &drawable)
                {
                    MaterialPolitic::Render(scene, scene->ModelMatrix(), drawable);
                }

                /** Render the given drawable with the scene and the given model matrix using the Politic */
                template<bool INDEX, typename ConfigPolitic>
                void    Render(ISceneGraph *scene, const TMatrix &modelMatrix, Drawable<VertexType, INDEX, ConfigPolitic> &drawable)
                {
                    MaterialPolitic::Render(scene, modelMatrix, drawable);
                }
        };

        // explicit instantiation for basic types
        /*
		static template class Material<BasicVertexType::Colored>;               ///< explicit instanciation of a DefaultMaterial colored
        static template class Material<BasicVertexType::Colored2d>;             ///< explicit instanciation of a DefaultMaterial colored 2d
        static template class Material<BasicVertexType::Textured>;              ///< explicit instanciation of a DefaultMaterial textured
        static template class Material<BasicVertexType::Textured2d>;            ///< explicit instanciation of a DefaultMaterial textured 2d
        static template class Material<BasicVertexType::Textured3d>;            ///< explicit instanciation of a DefaultMaterial textured 3d
		*/
		template class Material<BasicVertexType::Colored>;               ///< explicit instanciation of a DefaultMaterial colored
        template class Material<BasicVertexType::Colored2d>;             ///< explicit instanciation of a DefaultMaterial colored 2d
        template class Material<BasicVertexType::Textured>;              ///< explicit instanciation of a DefaultMaterial textured
        template class Material<BasicVertexType::Textured2d>;            ///< explicit instanciation of a DefaultMaterial textured 2d
        template class Material<BasicVertexType::Textured3d>;            ///< explicit instanciation of a DefaultMaterial textured 3d
    }
}

#endif
