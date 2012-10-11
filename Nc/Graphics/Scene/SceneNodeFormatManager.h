
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

    File Created At:        27/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_SCENE_SCENENODEFORMATMANAGER_H_
#define NC_GRAPHICS_SCENE_SCENENODEFORMATMANAGER_H_

#include "ISceneNodeFormatPlugin.h"

namespace Nc
{
    namespace Graphic
    {
        /// Manage a list of SceneNodeFormatPlugin and load scene node files.
        /**
            Plugins are loaded with the Xml System::Config file by using the "SceneNodeFormatPlugin" path in the "GameConf".

            <div class="title">Exemple of config file</div>
 \code
<Config>
    <GameConf>
        <SceneNodeFormatPlugin name="Nc:Plugin:libPluginFileFormatCollada.so" />
        <SceneNodeFormatPlugin name="Nc:Plugin:libPluginFileFormat3ds.so" />
    </GameConf>
    <ResourcesPath>
        <Plugin path="../Resources/Plugin/" />
    </ResourcesPath>
</Config>
 \endcode
        */
        class LGRAPHICS SceneNodeFormatManager
        {
            private:
                typedef std::list<System::PluginLoader<ISceneNodeFormatPlugin> >     ListSceneNodeFormat;

            public:
                SceneNodeFormatManager();
                ~SceneNodeFormatManager();

                /** Load the given \p file and return the loaded SceneNode. */
                ISceneNode              *Load(const Utils::FileName &file);

            private:
                /** \return the plugin associated to the given \p extension. If no plugin is found, then return. */
                ISceneNodeFormatPlugin  *GetSceneNodeFormatByExtension(const std::string &extension);

            private:
                ListSceneNodeFormat     _listSceneNodeFormat;           ///< list of SceneNodeFormatPlugin.
        };
    }
}

#endif
