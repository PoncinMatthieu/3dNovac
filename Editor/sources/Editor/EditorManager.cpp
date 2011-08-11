
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

#include "EditorManager.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::Editor;

EditorManager::EditorManager(Interface::MainWindow *mainWin)
    : QObject(NULL), _GUI(mainWin), _currentProject(NULL)
{
    CreateThreadManager();
}

EditorManager::~EditorManager()
{
    _threadManager->Stop();
    LOG << "Wait engines" << std::endl;
    _threadManager->Wait();

    LOG << "Destroy current project" << std::endl;
    if (_currentProject != NULL)
      delete _currentProject;
    delete _threadManager;
}

void EditorManager::CreateThreadManager()
{
    _threadManager = new QManager();

    // create graphical engine
    _graphic = new QGraphicEngine(_threadManager); // graphic engine
    _threadManager->AddEngine("Graphic", _graphic);

    // create editor engine
    _editor = new EditorEngine(_graphic, _threadManager); // game engine
    _editor->LimitFrameRate(60);
    _threadManager->AddEngine("Editor", _editor);
}

void EditorManager::Start()
{
    _threadManager->Start();

    // Load the default project
    _currentProject = new Project(std::string("Default-Project"), this);
    _currentProject->LoadDefaultProject();
}

void    EditorManager::SlotMoveNode(Nc::Graphic::ISceneNode *into, int pos, Nc::Graphic::ISceneNode *node, Nc::Graphic::ISceneNode *parent, int oldPos)
{
    _editor->PushEvent(EditorEngine::IndexMoveNode, EditorEngine::MoveStruct(into, pos, node, parent, oldPos));
}

void    EditorManager::DestroyCurrentSample()
{
    // destroy the last sample plugin and disable the scenes of the graphic engine
    if (!_pluginLoader.first.empty())
        _threadManager->RemoveEngine(_pluginLoader.first);
    for (Graphic::ListPScene::iterator it = _graphic->GetSceneManager()->Scenes().begin(); it != _graphic->GetSceneManager()->Scenes().end(); ++it)
        (*it)->Disable();
    _pluginLoader.first.clear();
}

void    EditorManager::SlotLoadSample(const std::string &filename)
{
    DestroyCurrentSample();

    // load the sample
    LOG << "Loading MainEnginePlugin `" + filename + "` \t\t";
    _pluginLoader.first = filename;
    _pluginLoader.second.Load(filename);
    LOG << *_pluginLoader.second << std::endl;
    _pluginLoader.second->SetManager(_threadManager);
    _pluginLoader.second->SetGraphicEngine(_graphic);
    _threadManager->AddEngine(filename, *_pluginLoader.second, Engine::Manager::Nop);
    _pluginLoader.second->Start();
    _pluginLoader.second.UnsetInstance();
    LOG << "DONE" << std::endl;
}
