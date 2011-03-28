
/*--------------------------------------------------------------------------------

	3dNovac System
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/

    cree le :        26/02/2011

--------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

	Description : the material config for the lighting material
                  the lighting material use a Normal Map. If the normal map is set,
                  we render the mesh with bump mapping

--------------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_LIGHTINGMATERIALCONFIG_H_
#define NC_GRAPHICS_LIGHTINGMATERIALCONFIG_H_

#include "MaterialConfig.h"

namespace Nc
{
    namespace GL
    {
        #pragma pack(push, 1) // pragma pack permet de supprimer l'optimisation du processeur et d'avoir un sizeof() correct
        struct LightingVertex
        {
            float           coord[3];     // position
            float           texture[2];   // coord de texture
            unsigned char   color[3];     // couleur
            float           norm[3];      // coord de normale
            float           bin[3];       // coord binormal
            float           tan[3];       // coord tangente

            static const VertexDescriptor   &GetDescriptor();
            inline void                     Transform(const TMatrix &m)     {m.Transform(coord);}
        };
        #pragma pack(pop)
    }

    namespace Graphic
    {
        void   LGRAPHICS ComputeLocalSpace(GL::LightingVertex &v1, GL::LightingVertex &v2, GL::LightingVertex &v3);

        struct LGRAPHICS LightingMaterialConfig : public MaterialConfig
        {
            LightingMaterialConfig() : GhostMode(false)        {}
            LightingMaterialConfig(const GL::Texture &texture, const GL::Texture &normal)
                : MaterialConfig(texture), NormalMap(normal), GhostMode(false)    {}
            virtual ~LightingMaterialConfig()                   {}

            virtual inline  MaterialConfig *Clone() const   {return new LightingMaterialConfig(*this);}

            GL::Texture     NormalMap;          ///< the normal map for bump mapping
            bool            GhostMode;          ///< if true, only the light pass is draw
        };
    }
}


#endif // NC_LIGHTINGMATERIALCONFIG_H_
