
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

#include "SceneNodeFormatManager.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::System;
using namespace Nc::Graphic;

SceneNodeFormatManager::SceneNodeFormatManager()
{
    Xml::Object *gameConf = CONFIG->Block("GameConf");
    for (Xml::ListObject::iterator it = gameConf->ListChild().begin(); it != gameConf->ListChild().end(); ++it)
    {
        if ((*it)->Name() == "SceneNodeFormatPlugin")
        {
            const std::string &name = (*it)->Param("name");
            LOG << "Loading SceneNodeFormatPlugin `" + name + "` \t\t";
            _listSceneNodeFormat.push_back(System::PluginLoader<SceneNodeFormatPlugin>());
            _listSceneNodeFormat.rbegin()->Load(name);
            LOG << "DONE" << std::endl;
        }
    }
}

SceneNodeFormatManager::~SceneNodeFormatManager()
{
}

ISceneNode *SceneNodeFormatManager::Load(const FileName &file)
{
    SceneNodeFormatPlugin *sceneNodeFormat = GetSceneNodeFormatByExtension(file.Extension());
    if (sceneNodeFormat)
        return sceneNodeFormat->Load(file);
    else
        throw Utils::Exception("SceneNodeFormatManager", "Can't find suitable plugin for the extension file format: `" + file.Extension() + "`");
    return NULL;
}

SceneNodeFormatPlugin    *SceneNodeFormatManager::GetSceneNodeFormatByExtension(const std::string &extension)
{
    for (ListSceneNodeFormat::iterator it = _listSceneNodeFormat.begin(); it != _listSceneNodeFormat.end(); ++it)
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
