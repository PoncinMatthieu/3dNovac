
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        27/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_OBJECT_3D_MESHMANAGER_H_
#define NC_GRAPHICS_OBJECT_3D_MESHMANAGER_H_

#include "Mesh.h"

namespace Nc
{
    namespace Graphic
    {
        class LGRAPHICS MeshManager
        {
            private:
                //typedef std::map<MeshProcessor*>  MapMeshProcessor;

            public:
                MeshManager();
                ~MeshManager();

                Object3d    *Load(const Utils::FileName &file);

            private:
                //MeshProcessor   *GetProcessorByExtension(const std::string &extension);

                //MapMeshProcessor        _mapProcessor;
        };
    }
}

#endif // NC_GRAPHICS_OBJECT_3D_MESHMANAGER_H_
