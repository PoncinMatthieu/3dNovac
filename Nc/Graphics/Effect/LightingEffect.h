
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

    File Created At:        01/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_EFFECT_LIGHTINGEFFECT_H_
#define NC_GRAPHICS_EFFECT_LIGHTINGEFFECT_H_

#include "Effect.h"
#include "../Object/Object.h"
#include "../Material/IMaterial.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a light to use with the LightingEffect.
        struct LIB_NC_GRAPHICS Light : public Object
        {
            NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::Object, System::Object, Nc::Graphic::Light)

            Light()
                : Object(), color(0.5f, 0.5f, 0.5f, 0), radius(10)        {}
            Light(const Color &c, const float r = 10)
                : Object(), color(c), radius(r)                           {}

            Color       color;              ///< the color of the light.
            float       radius;             ///< the raduis of the light.
        };

        /// Define an interface for the lighting Material.
        class ILightingMaterial : public IMaterial
        {
            public:
                ILightingMaterial(const std::string &name) : IMaterial(name), _currentLightingEffect(NULL)     {}

                /** Set the current LightingEffect, to retreive some informations like a list of Lights. */
                void SetCurrentLightingEffect(LightingEffect *effect)   {_currentLightingEffect = effect;}

            protected:
                LightingEffect      *_currentLightingEffect;    ///< pointer to the current lighting effect.
        };

        /// Define a LightinhEffect used to render some light effect on childs.
        /**
            The lightingEffect is a "Material effect" in that case, we need to configure the materials of the childs to apply the effects.
        */
        class LIB_NC_GRAPHICS LightingEffect : public Effect
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Entity, System::Object, Nc::Graphic::LightingEffect)

            public:
                LightingEffect(ILightingMaterial *material);
                ~LightingEffect();

                virtual ISceneNode          *Clone() const              {return new LightingEffect(*this);}

                /** \return the color ambiant of the lighting. */
                inline const Color          &ColorAmbiant() const       {return _colorAmbiant;}
                /** \return the lights. */
                inline const ListPLight     &Lights() const             {return _listLight;}
                /** \return the lights. */
                inline ListPLight           &Lights()                   {return _listLight;}
                /** \return the lighting material. */
                inline ILightingMaterial    *Material()                 {return _material;}

            protected:
                /** Render the childs. */
                virtual void                Render(SceneGraph *scene);

            private:
                ILightingMaterial           *_material;         ///< the lighting material.
                Color                       _colorAmbiant;      ///< the color ambiant.
                ListPLight                  _listLight;         ///< the list of light.
        };
    }
}


#endif
