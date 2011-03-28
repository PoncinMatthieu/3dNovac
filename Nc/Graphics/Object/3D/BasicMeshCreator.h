
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
/*-----------------------------------------------------------------------------

    namespace to create some basic mesh

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHIC_BASICMESHCREATOR_H_
#define NC_GRAPHIC_BASICMESHCREATOR_H_

#include "Mesh.h"

namespace Nc
{
    namespace Graphic
    {
        namespace LGRAPHICS BasicMeshCreator
        {
            // cree un repere :   rouge:X     bleu:Y      vert:Z
            Object3d *Repere(float echelle, const Vector3f &center = Vector3f(0,0,0));
            Object3d *Repere(const Vector3f &echelle, const Vector3f &center = Vector3f(0,0,0));
        }
    }
}

#endif
