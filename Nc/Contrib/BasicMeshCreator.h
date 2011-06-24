
/*-----------------------------------------------------------------------------

	3dNovac Contrib
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CONTRIB_BASICMESHCREATOR_H_
#define NC_CONTRIB_BASICMESHCREATOR_H_

#include <Nc/Graphics/Object/Object.h>
#include "Define.h"

namespace Nc
{
    namespace Graphic
    {
        namespace BasicMeshCreator
        {
            /**
                Create a plan
                \param size : the 2d dimension size
                \param texture : the texture of the plan
                \return the resulting Object3d
            */
            LCONTRIB Object     *Plan(const Vector2f &size, const GL::Texture &texture);

            /**
                Create a grid
                \param size to specified the 2d size of the whole grid. A case has the size 1*1. The z coord is to specified the height of the grid
                \param c the color of the grid lines
                \return the resulting Object3d
            */
            LCONTRIB Object     *Grid(const Vector3f &size, const Color &c);
        }
    }
}

#endif
