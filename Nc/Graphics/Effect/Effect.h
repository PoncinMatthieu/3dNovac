
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

    File Created At:        16/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_EFFECT_EFFECT_H_
#define NOVAC_GRAPHIC_EFFECT_EFFECT_H_

#include "../Scene/SceneNode.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to manage and render effects
        class LGRAPHICS Effect : public Entity
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Entity);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

            public:
                Effect(const char *className)
                    : Entity(className), _activated(true)          {}
                virtual ~Effect()                                   {}

                static const char       *ClassName()                {return "Effect";}
                virtual ISceneNode      *Clone() const = 0;

                inline bool             Activated() const           {return _activated;}
                inline void             Activated(bool state)       {_activated = state;}

                /** To Display the effect */
                virtual void            Render(SceneGraph *scene) = 0;

            protected:
                bool        _activated;
        };

        // specialization of the method GetNode<> for an IObject
        //template<>
        //Effect *ISceneNode::GetNode<>();
    }
}

#endif // Effect_H_INCLUDED
