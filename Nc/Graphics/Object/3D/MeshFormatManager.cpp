
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

    File Created At:        10/04/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "MeshFormatManager.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::Graphic;

MeshFormatManager::MeshFormatManager()
{
    Xml::Object *gameConf = CONFIG->Block("GameConf");
    for (Xml::ListObject::iterator it = gameConf->ListChild().begin(); it != gameConf->ListChild().end(); ++it)
    {
        if ((*it)->Name() == "MeshFormatPlugin")
        {
            const std::string &name = (*it)->Param("name");
            LOG << "Loading MeshFormatPlugin `" + name + "` \t\t";
            _listMeshFormat.push_back(System::PluginLoader<MeshFormatPlugin>());
            _listMeshFormat.rbegin()->Load(name);
            LOG << "DONE" << std::endl;
        }
    }
}

MeshFormatManager::~MeshFormatManager()
{
}

Object3d *MeshFormatManager::Load(const FileName &file)
{
    MeshFormatPlugin *meshFormat = GetMeshFormatByExtension(file.Extension());
    if (meshFormat)
        return meshFormat->Load(file);
    else
        throw Utils::Exception("MeshFormatManager", "Can't find suitable plugin for the extension file format: `" + file.Extension() + "`");
    return NULL;
}

MeshFormatPlugin    *MeshFormatManager::GetMeshFormatByExtension(const std::string &extension)
{
    for (ListMeshFormat::iterator it = _listMeshFormat.begin(); it != _listMeshFormat.end(); ++it)
    {
        bool compatibility = false;
        std::string exts = (*it)->GetAssociatedExtension();
        std::string arg;

        while (!exts.empty() && !compatibility && Utils::Convert::SplitStringTo(exts, ";", arg))
        {
            if (arg == extension)
                compatibility = true;
        }

        if (compatibility)
            return it->GetInstance();
    }
    return NULL;
}

