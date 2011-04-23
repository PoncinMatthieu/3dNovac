
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        28/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_OBJECT_3D_MESHFORMATPLUGIN_H_
#define NC_GRAPHICS_OBJECT_3D_MESHFORMATPLUGIN_H_

#include <Nc/Core/System/PluginLoader.h>
#include "Mesh.h"

namespace Nc
{
    namespace Graphic
    {
        /// Interface to create a plugin that will be used to import mesh in any mesh format
        class LGRAPHICS MeshFormatPlugin
        {
            public:
                MeshFormatPlugin(const std::string &extension) : _extension(extension)      {}
                virtual ~MeshFormatPlugin() {}

                /** Return the associated extensions in this format: "dae;obj;3ds" */
                inline const std::string   &GetAssociatedExtension()                {return _extension;}

                /** Load the file */
                virtual Object3d    *Load(const Utils::FileName &file) = 0;
                /** Save the file */
                virtual void        Save(const Utils::FileName &file, Object3d *obj) = 0;

            protected:
                std::string     _extension;             ///< the extension supported should be in this format "dae;obj;3ds"
        };
    }
}

#endif
