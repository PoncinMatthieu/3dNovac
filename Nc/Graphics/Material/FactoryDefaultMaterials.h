
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

#ifndef NC_GRAPHICS_MATERIAL_FACTORYDEFAULTMATERIALS_H_
#define NC_GRAPHICS_MATERIAL_FACTORYDEFAULTMATERIALS_H_

#include "DefaultMaterials.h"

namespace Nc
{
    namespace Graphic
    {
        /// Factory of DefaultMaterial, used to create/intialize/choose the best material for a set of drawables.
        class LIB_NC_GRAPHICS FactoryDefaultMaterials : public Utils::Singleton<FactoryDefaultMaterials>
        {
            public:
                typedef std::pair<unsigned int,IDefaultMaterial*>   PriorityMaterial;
                typedef std::list<PriorityMaterial>                 PriorityMaterialList;

            public:
                FactoryDefaultMaterials()       {}
                ~FactoryDefaultMaterials();

                /** \return the best material for a set of drawables. */
                IDefaultMaterial        *GetBestMaterial(const GL::DrawableArray &drawables);
                /** \return the best material for the given \p drawable. */
                IDefaultMaterial        *GetBestMaterial(const GL::Drawable *drawable);

                /** Create, Add and return a DefaultMaterial with the given type of DefaulMaterial. */
                template<typename DefaultMaterialType>
                DefaultMaterialType     *AddDefaultMaterial()
                {
                    DefaultMaterialType *material = new DefaultMaterialType();
                    _materials.push_back(PriorityMaterial(DefaultMaterialType::GetDescriptorPriority(),material));
                    _materials.sort(CmpPriority);
                    return material;
                }

            private:
                /** \return the best default material for the given vertex descriptor. */
                IDefaultMaterial        *GetBestMaterial(GL::VertexDescriptor *descriptor);

                // ordre decroissant
                static bool CmpPriority(const PriorityMaterial &first, const PriorityMaterial &second)
                {
                    return (first.first > second.first);
                }

                PriorityMaterialList        _materials;     ///< a list of priority for each default material.
        };
	}

    namespace Utils
    {
        // explicit instanciation
        template class Singleton<Graphic::FactoryDefaultMaterials>;
    }
}

#endif
