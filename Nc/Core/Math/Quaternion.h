
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        13/06/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_MATH_QUATERNION_H_
#define NC_CORE_MATH_QUATERNION_H_

#include "../Define.h"

namespace Nc
{
    namespace Math
    {
        /// Define a Quaternion that can be used to manage rotation.
        struct Quaternion
        {
            Quaternion();
            Quaternion(float x, float y, float z, float w);

            float   x;
            float   y;
            float   z;
            float   w;
        };
    }
}

#endif

