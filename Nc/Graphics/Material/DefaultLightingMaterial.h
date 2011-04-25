
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

    File Created At:        04/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_MATERIAL_DEFAULTLIGHTINGMATERIAL_H_
#define NC_GRAPHICS_MATERIAL_DEFAULTLIGHTINGMATERIAL_H_

#include <Nc/Core/Utils/Mask.h>
#include "Material.h"

namespace Nc
{
    namespace Graphic
    {
        /// The material config for the defaultLighting method
        struct LGRAPHICS DefaultLightingMaterialConfigPolitic
        {
            GL::Texture     texture;
            GL::Texture     normalMap;
        };

        /// The material politic used to render a defaultLighting
        /**
            \todo Manage specular components
            \todo Find a way to draw advanced effect like shadow and reflexion
        */
        class LGRAPHICS DefaultLightingMaterialPolitic
        {
            public:
                /// Pattern of the DefaultLighting rendering method
                enum MaterialPattern
                {
                    BumpMapping = 1 << 0,     ///< Display the drawable using normal mapping (only if the material config has a normalMap)
                    DisplayNormal = 1 << 1      ///< Display the normals (only in debug)
                };

            public:
                DefaultLightingMaterialPolitic();
                ~DefaultLightingMaterialPolitic();

                /** Configure the geometry with the suitable attributes according to the shader */
                void    Configure(GL::IGeometryBuffer<BasicVertexType::Textured> &geometry);
                /** Render the drawable */
                void    Render(ISceneGraph *scene, TMatrix &modelMatrix, IDrawable<BasicVertexType::Textured, DefaultLightingMaterialConfigPolitic> &drawable);

                /** Return the pattern */
                Utils::Mask<MaterialPattern>    &Pattern()      {return _patternMask;}

            protected:
                Utils::Mask<MaterialPattern>    _patternMask;           ///< the pattern mask of the material

                GL::Shader      _shader;                                ///< the shader of the material
                GL::Texture     _lightMap;                              ///< the light map to used a perpixel lighting method

                unsigned int    _uniformM;
                unsigned int    _uniformPV;
                unsigned int    _uniformLight;
                unsigned int    _uniformLightColor;
                unsigned int    _uniformLightPass;
                unsigned int    _uniformTextured;
                unsigned int    _uniformDiffuse;
                unsigned int    _uniformLightMap;
                unsigned int    _uniformBumpMapping;
                unsigned int    _uniformNormalMap;
        };

        // instanciation explicit
        //static template class Material<BasicVertexType::Textured, DefaultLightingMaterialPolitic>;      ///< explicit instanciation of the material with DefaultLightingPolitic
		template class Material<BasicVertexType::Textured, DefaultLightingMaterialPolitic>;      ///< explicit instanciation of the material with DefaultLightingPolitic
    }
}

#endif
