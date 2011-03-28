
/*--------------------------------------------------------------------------------

	3dNovac Engine
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        22/05/2010

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

                 Implementation de la classe "LightingMaterial"

    herite de Material, permet de rendre l'objet avec une liste de lumiere
        rendu avec light-map, et avec-ou-sans bump-mapping simple

--------------------------------------------------------------------------------*/


#ifndef NC_GRAPHIC_LIGHTINGMATERIAL_H_
#define NC_GRAPHIC_LIGHTINGMATERIAL_H_

#include "../../Define.h"
#include "Material.h"
#include "LightingMaterialConfig.h"

namespace Nc
{
    namespace Graphic
    {
        class Light;

        class LGRAPHICS LightingMaterial : public Material, public Utils::Singleton<LightingMaterial>
        {
            public:
                LightingMaterial();
                virtual ~LightingMaterial();

                virtual MaterialConfig  *CreateAssociatedMaterialConfig()                    {return new LightingMaterialConfig();}
                virtual void            Render(Mesh *m, const MaterialConfig *config);

                bool            DrawLocalSpace;         ///< if true, draw the local space of the Meshs
                bool            BumpMapping;            ///< if true, render the meshs with bumpmapping

            protected:
                void            RenderLocalSpace(Mesh *m);

                GL::Shader      _lightShader;           ///< the light shader used to render the light pass
                GL::Texture     _lightMap;              ///< the light map used to render the light pass, (it's a 3d sphere map to show attenuation)
        };
    }
}

#endif
