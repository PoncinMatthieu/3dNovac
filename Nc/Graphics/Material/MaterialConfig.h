
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

    File Created At:        26/02/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_MATERIAL_MATERIALCONFIG_H_
#define NC_GRAPHICS_MATERIAL_MATERIALCONFIG_H_

#include "../../Define.h"
#include "../Core/GL/Blend.h"
#include "../Core/GL/Texture.h"
#include "BasicVertexType.h"

namespace Nc
{
    namespace Graphic
    {
        /// Default template MaterialConfig Politics
        /**
            For mode detail show the specialisations of the class
        */
        template<typename VertexType>
        struct DefaultMaterialConfigPolitics    {};

        /// Base structure for Material configuration to set properties in the rendering of a Geometry.
        /**
            Each geometry needs a MaterialConfig to be render with a Material
            material config set store properties like blend, deapth test, texture ....
        */
        struct LGRAPHICS IMaterialConfig
        {
            IMaterialConfig() : depthTest(true)         {}
            IMaterialConfig(GL::Blend::Pattern &p) : blend(p), depthTest(true)  {}
            ~IMaterialConfig()                          {}

            /** Set the blend method */
            void SetBlend(const GL::Blend::Pattern &p)    {blend.SetPattern(p);}

            /** Enable the default basic config (blend and depthTest) */
            void Enable()
            {
                blend.Enable();
                if (!depthTest)
                    glDisable(GL_DEPTH_TEST);
            }
            /** Disable the default basic config (blend and depthTest) */
            void Disable()
            {
                blend.Disable();
                if (!depthTest)
                    glEnable(GL_DEPTH_TEST);
            }

            GL::Blend   blend;          ///< the blend methode of the material
            bool        depthTest;      ///< true if the depth test should be enable
        };

        /// Material configuration
        template<typename VertexType, typename ConfigPolitic = DefaultMaterialConfigPolitics<VertexType> >
        struct  MaterialConfig : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                {}
            MaterialConfig(GL::Blend::Pattern p) : IMaterialConfig(p)       {}
        };


        /// Specialisation of Material configuration for a Textured VertexType
        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), texture(t)    {}

            GL::Texture     texture;
        };

        /// Specialisation of Material configuration for a Textured2d VertexType
        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured2d, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), texture(t)    {}

            GL::Texture     texture;
        };

        /// Specialisation of Material configuration for a Textured3d VertexType
        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured3d, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), texture(t)    {}

            GL::Texture     texture;
        };

        // explicit instantiation for basic types
        static template class MaterialConfig<BasicVertexType::Colored>;             ///< Explicite instanciation of DefaulMaterialConfig Colored
        static template class MaterialConfig<BasicVertexType::Colored2d>;           ///< Explicite instanciation of DefaulMaterialConfig Colored2d
        static template class MaterialConfig<BasicVertexType::Textured>;            ///< Explicite instanciation of DefaulMaterialConfig Textured
        static template class MaterialConfig<BasicVertexType::Textured2d>;          ///< Explicite instanciation of DefaulMaterialConfig Textured2d
        static template class MaterialConfig<BasicVertexType::Textured3d>;          ///< Explicite instanciation of DefaulMaterialConfig Textured3d
    }
}

#endif
