
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

    File Created At:        04/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_2D_OBJECT2D_H_
#define NC_GRAPHIC_2D_OBJECT2D_H_

#include "../Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Define a 2d Object to rendering in 2d
        class LGRAPHICS   Object2d : public Object
        {
            public:
                Object2d();
                Object2d(const TMatrix &m);
                virtual ~Object2d();
        };
    }
}

#endif
