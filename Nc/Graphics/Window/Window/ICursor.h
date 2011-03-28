
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Provide an interface to manage a cursor

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_WINDOW_ICURSOR_H_
#define NC_GRAPHICS_WINDOW_ICURSOR_H_

#include <Nc/Core/Utils/NonCopyable.h>
#include <Nc/Core/Math/Math.h>
#include "../../Define.h"

namespace Nc
{
    namespace Graphic
    {
        class LCORE ICursor : Utils::NonCopyable
        {
            public:
                ICursor()               {}
                virtual ~ICursor()      {}

                virtual void Enable() = 0;
                virtual void Disable() = 0;

                /// load data from 1 bit per pixel, TODO: challenge do better !
                virtual void LoadFromData(const unsigned char *data, const unsigned char *mask, const Math::Vector2ui &size, const Math::Vector2ui &posCenter) = 0;
                //virtual void LoadFromImage(const Utils::FileName &f) = 0; //todo
                virtual void LoadFromXpm(const char *xpm[]) = 0;
        };
    }
}

#endif
