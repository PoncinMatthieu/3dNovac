
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Project.h"
#include "EditorManager.h"

using namespace Nc;
using namespace Nc::Editor;

Project::Project(const std::string &name, EditorManager *editorManager)
    : _name(name), _editorManager(editorManager), _loaded(false)
{
}

Project::Project(const Nc::Utils::FileName &file, EditorManager *editorManager)
    : _name(file.ShortFilename()), _file(file), _editorManager(editorManager), _loaded(false)
{
    LoadProject();
}

Project::~Project()
{
    ClearProject();
}

void Project::ClearProject()
{
    if (!_loaded)
        return;
    _libraryPath = "./";
    _loaded = false;
}

void Project::LoadDefaultProject()
{
    ClearProject();

    _libraryPath = CONFIG->Block("RessourcesPath")->Line("Library")->Param("path");
    // creation d'une scene vide
    //_editorManager->Editor()->PushEvent(EditorEngine::IndexCreateNewScene, std::string("Default-Scene"));
    _loaded = true;
}

void Project::LoadProject()
{
///\todo Project::LoadProject not implemented
    LOG_DEBUG << "TODO: Not implemented" << std::endl;
    _loaded = true;
}
