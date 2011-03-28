
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
/*-----------------------------------------------------------------------------

	Description : Material configuration class to set basic properties of a material
                  Each Material needs a MaterialConfig to render a mesh,
                  so the material configs are contained by the mesh
                  and used in the render pass
                  if the _texture is set, the material will use it

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
        template<typename VertexType>
        struct DefaultMaterialConfigPolitics    {};

        struct LGRAPHICS IMaterialConfig
        {
            IMaterialConfig() : DepthTest(true)                 {}
            IMaterialConfig(GL::Blend::Pattern &p) : Blend(p), DepthTest(true)  {}
            ~IMaterialConfig()                          {}

            void Enable()
            {
                Blend.Enable();
                if (!DepthTest)
                    glDisable(GL_DEPTH_TEST);
            }
            void Disable()
            {
                Blend.Disable();
                if (!DepthTest)
                    glEnable(GL_DEPTH_TEST);
            }

            GL::Blend   Blend;
            bool        DepthTest;
        };

        template<typename VertexType, typename ConfigPolitic = DefaultMaterialConfigPolitics<VertexType> >
        struct  MaterialConfig : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                {}
            MaterialConfig(GL::Blend::Pattern p) : IMaterialConfig(p)       {}
        };


        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), Texture(t)    {}

            GL::Texture     Texture;
        };

        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured2d, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), Texture(t)    {}

            GL::Texture     Texture;
        };

        template<typename ConfigPolitic>
        struct  MaterialConfig<BasicVertexType::Textured3d, ConfigPolitic> : public IMaterialConfig, public ConfigPolitic
        {
            MaterialConfig()                                                                        {}
            MaterialConfig(GL::Blend::Pattern p, GL::Texture t) : IMaterialConfig(p), Texture(t)    {}

            GL::Texture     Texture;
        };
    }
}

#endif
