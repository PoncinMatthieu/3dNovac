
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
#include "../Core/GL/Program.h"
#include "../Core/GL/Texture.h"
#include "LightingEffect.h"

namespace Nc
{
    namespace Graphic
    {
        /// The material politic used to render a defaultLighting
        /**
            \todo Manage specular components
            \todo Find a way to draw advanced effect like shadow and reflexion
        */
        class LGRAPHICS DefaultLightingMaterial : public ILightingMaterial
        {
            public:
                /// Pattern of the DefaultLighting rendering method
                enum MaterialPattern
                {
                    Disabled = 1 << 0,          ///< No lighting
                    BumpMapping = 1 << 1,       ///< Display the drawable using normal mapping (only if the material config has a normalMap)
                    DisplayNormal = 1 << 2      ///< Display the normals (only in debug)
                };

            private:
                enum Uniforms
                {
                    UniformM = 0,
                    UniformPV,
                    UniformNormalMatrix,
                    UniformLight,
                    UniformLightPass,
                    UniformLightColor,
                    UniformTextured,
                    UniformDiffuse,
                    UniformLightMap,
                    UniformNormalMap,
                    UniformBumpMapping,
                    NbUniform
                };

            public:
                DefaultLightingMaterial();
                ~DefaultLightingMaterial();

                /** Configure the geometry with the suitable attributes according to the shader */
                virtual bool    Configure(Drawable &drawable);

                /** \return the pattern */
                Utils::Mask<MaterialPattern>    &Pattern()      {return _patternMask;}

            protected:
                /** Render the given drawable with the given model matrix */
                virtual void    Render(SceneGraph *scene, const TMatrix &modelMatrix, Drawable &drawable);

            protected:
                Utils::Mask<MaterialPattern>    _patternMask;               ///< the pattern mask of the material

                GL::Program                     _program;                   ///< the program shader of the material
                Array<unsigned int, NbUniform>  _uniforms;                  ///< the tab of uniform locations
                Array<unsigned int, 4>          _attribs;                   ///< the tab of attrib locations
                GL::Texture                     _lightMap;                  ///< the light map to used a perpixel lighting method

                GL::Blend                       _blend;                     ///< set the blending for the light pass
                GL::RasterMode                  _rasterMode;                ///< disable the depth mask for the light pass
        };
    }
}

#endif
