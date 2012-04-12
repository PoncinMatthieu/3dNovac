
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

    File Created At:        25/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_EFFECT_ANIMATION_H_
#define NOVAC_GRAPHIC_EFFECT_ANIMATION_H_

#include <vector>
#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to manage and render animations
        class LGRAPHICS     Animation : public Effect
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Effect, System::Object, Nc::Graphic::Animation);

            public:
                Animation()
                    : Effect(), _alive(true)                {}

                /** Start the effect */
                virtual void            Start()             {}
                /** Stop the effect */
                virtual void            Stop()              {}
                /** \return true if the effect is started */
                virtual bool            Started() const     {return true;}

                /** Set alive statement */
                void                Alive(bool state)           {_alive = state;}
                /** \return the alive statement */
                virtual inline bool Alive() const               {return _alive;}

            protected:
                bool            _alive;                         ///< statement to specifiy if the effect is dead (terminate), in that case we should destroy the animation upstream
        };
    }
}

#endif
