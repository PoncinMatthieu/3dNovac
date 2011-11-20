
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

    File Created At:        27/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_BASICMESHCREATOR_H_
#define NC_GRAPHIC_BASICMESHCREATOR_H_

#include "Object.h"

namespace Nc
{
    namespace Graphic
    {
        /// Reference some usefull function used to create simple Mesh like Axis
        namespace BasicMeshCreator
        {
            /**
                Create an axis with this color sheme : Red:X  Blue:Y Green:Z.
                If \p withLegend is true, the object will also contain 3 label X,Y,Z
                \return the corresponding Mesh
            */
            LGRAPHICS Object *Axis(float scale, bool withLegend = true, const Vector3f &center = Vector3f(0,0,0));

            /**
                Create a axis with this color sheme : Red:X  Blue:Y Green:Z.
                If \p withLegend is true, the object will also contain 3 label X,Y,Z
                \return the corresponding Mesh
            */
            LGRAPHICS Object *Axis(const Vector3f &scale, bool withLegend = true, const Vector3f &center = Vector3f(0,0,0));

            /**
                Add a Graphic::String controled with a Billboard effect to the given \p entity as child at the given \p position.
                \return the created Graphic::String
            */
            LGRAPHICS String *AddLabel(const std::string &text, float caracterSize, const Color &color, const std::string &fontName, bool centerText, Entity *entity, const Vector3f &position = Vector3f(0,0,0));
        }
    }
}

#endif
