
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

#include "../Object/Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to manage and render effect and animation
        class LGRAPHICS   Effect : public Object
        {
            public:
                Effect()                                        {_alive = true;}
                virtual ~Effect()                               {}

                /** copy the effect */
                virtual Effect  *Clone() const = 0;

                /** Start the effect */
                virtual void    Start() = 0;
                /** Stop the effect */
                virtual void    Stop() = 0;
                /** Return true if the effect is started */
                virtual bool    Started() const = 0;

                /** To Update the effect */
                virtual void    Update(float runningTime) = 0;
                /** To Display the effect */
                virtual void    Render(ISceneGraph *scene) = 0;

                /** Set alive statement */
                void                Alive(bool state)           {_alive = state;}
                /** Return the alive statement */
                virtual inline bool Alive() const               {return _alive;}

            protected:
                bool            _alive;                         ///< statement to specifiy if the effect is dead (terminate), in that case we could destroy the animation upstream
        };
    }
}

#endif // Effect_H_INCLUDED
