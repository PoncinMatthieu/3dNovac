
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

#include "EditorEngine.h"
#include "QManager.h"
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Graphics.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/GUI/GUI.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include <Nc/Contrib/BasicMeshCreator.h>
#include <Nc/Graphics/Effect/DefaultLightingMaterial.h>


using namespace std;
using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::Graphic;
using namespace Nc::GUI;
using namespace Nc::Editor;

EditorEngine::EditorEngine(Graphic::Engine *graphic, Nc::Engine::Manager *manager)
    : MainEngine(manager), _emitChanges(true), _graphic(graphic),
      _GUI(NULL), _currentScene(NULL), _currentNodeSelected(NULL),
      _lightingEffect(NULL)
{
    _name = "Editor Engine";
    _light = NULL;

    AddNewCmd(IndexCreateNewScene,      (Nc::Engine::CmdFunction)&EditorEngine::CreateNewSceneCmd);
    AddNewCmd(IndexSelectScene,         (Nc::Engine::CmdFunction)&EditorEngine::SelectSceneCmd);
    AddNewCmd(IndexSceneDeleted,        (Nc::Engine::CmdFunction)&EditorEngine::SceneDeletedCmd);
    AddNewCmd(IndexLoadNode,            (Nc::Engine::CmdFunction)&EditorEngine::LoadNodeCmd);
    AddNewCmd(IndexSelectNode,          (Nc::Engine::CmdFunction)&EditorEngine::SelectNodeCmd);
    AddNewCmd(IndexRemoveNode,          (Nc::Engine::CmdFunction)&EditorEngine::RemoveNodeCmd);
    AddNewCmd(IndexMoveNode,            (Nc::Engine::CmdFunction)&EditorEngine::MoveNodeCmd);
    AddNewCmd(IndexChangeRasterMode,    (Nc::Engine::CmdFunction)&EditorEngine::ChangeRasterModeCmd);
}

EditorEngine::~EditorEngine()
{
}

void EditorEngine::ReleaseContent()
{
    delete _lightingEffect->Material();
    delete _lightingEffect;
    // destroy the scenes
    for (ListPScene::iterator it = _scenes.begin(); it != _scenes.end(); ++it)
        delete (*it);
    Graphic::String::DestroyFonts();
    delete _context;
}

void EditorEngine::LoadContent()
{
    AddInput(_graphic->GetWindow()->GetInput());

    _GUI = new GUI::SceneGraph();
    Camera2d *camera = new Camera2d();
    _GUI->AddChild(camera);
    GUI::FPSWidget *fps = new GUI::FPSWidget();         // creation du widget pour afficher les fps
    _GUI->AddChild(fps);
    _graphic->GetSceneManager()->AddScene(_GUI);
    _scenes.push_back(_GUI);

    if (_emitChanges)
    {
        emit SignalScenesChanged(&Scenes());
        emit SignalCurrentSceneChanged(_currentScene);
    }

    // creation du LightingEffect
    DefaultLightingMaterial *lightMaterial = new DefaultLightingMaterial();
    _lightingEffect = new LightingEffect(lightMaterial);
    LOG << "create lighting effect: " << _lightingEffect << std::endl;
    _lightingEffect->ShouldBeDeleted(false);
    _lightingEffect->Lights().push_back(new Light(Color(0.1f, 0.1f, 0.1f), 100));   // ajout d'une light
}

bool EditorEngine::ExitEvent()
{
    emit SignalStop();  // send un signal pour indiquer que l'on veut quiter le game engine
    return true;        // confirme l'arret de l'engine
}

void EditorEngine::Update(float runningTime)
{
    for (ListPScene::iterator it = OurScenes().begin(); it != OurScenes().end(); ++it)
        (*it)->Update(runningTime);
    if (_currentScene != NULL && _currentScene->CurrentCamera() != NULL)
    {
        Camera3d *cam = _currentScene->CurrentCamera()->AsWithoutThrow<Camera3d>();
        if (cam != NULL && *_lightingEffect->Lights().begin() != NULL)
            (*_lightingEffect->Lights().begin())->Matrix.Translation(cam->Eye());
    }
}

void EditorEngine::ManageWindowEvent(Nc::System::Event &event)
{
    bool send = true;
    if (event.Type == System::Event::Resized)
    {
        if (_GUI != NULL && _GUI->CurrentCamera() != NULL)
            _GUI->CurrentCamera()->Resized(event);
        if (_currentScene != NULL && _currentScene->CurrentCamera() != NULL)
            _currentScene->CurrentCamera()->Resized(event);
    }
    if (event.Type == System::Event::KeyPressed)
    {
        // quit si ctrl-W
        if (event.Type == System::Event::KeyPressed && event.Key.Code == System::Key::W && WindowInput::KeyState(System::Key::LControl))
            Quit();
    }
    // send les evenements au gameManager (celui ci les dispatch a la GUI et au fonction Keybord/MouseEvent)
    if (send)
        MainEngine::ManageWindowEvent(event);
    if (_GUI != NULL)
        _GUI->ManageWindowEvent(event);
}

void EditorEngine::KeyboardEvent(System::Event &event)
{
    if (_currentScene != NULL && _currentScene->CurrentCamera())
        _currentScene->CurrentCamera()->KeyboardEvent(event);
    if (event.Type == System::Event::KeyPressed)
    {
        if (event.Key.Code == System::Key::F1)        // bump mapping
            static_cast<DefaultLightingMaterial*>(_lightingEffect->Material())->Pattern().Trigger(DefaultLightingMaterial::BumpMapping);
        #ifdef _DEBUG
        else if (event.Key.Code == System::Key::F2)        // draw les normal
            static_cast<DefaultLightingMaterial*>(_lightingEffect->Material())->Pattern().Trigger(DefaultLightingMaterial::DisplayNormal);
        #endif
    }
}

void EditorEngine::MouseButtonEvent(System::Event &event)
{
    if (_currentScene != NULL && _currentScene->CurrentCamera())
        _currentScene->CurrentCamera()->MouseButtonEvent(event);
}

void EditorEngine::MouseMotionEvent(System::Event &event)
{
    if (_currentScene != NULL && _currentScene->CurrentCamera())
        _currentScene->CurrentCamera()->MouseMotionEvent(event);
}

void EditorEngine::CreateNewScene(const std::string &name)
{
    if (name.empty())
        throw Utils::Exception("EditorEngine::CreateNewScene", "The name is empty");

    _currentNodeSelected = NULL;

    // creation de la scene
    Graphic::SceneGraph *scene = new Graphic::SceneGraph();
    scene->Name(name);

    // creation de la camera
    StandardCamera3d *camera = new StandardCamera3d(_graphic->GetWindow());
    camera->MoveButton(System::Mouse::Middle);
    scene->AddChild(camera);

    // ajout du repere
    Graphic::Object *repere = BasicMeshCreator::Repere(1);
    camera->AddChild(repere);

    // creation d'une grid
    Graphic::Object *grid = BasicMeshCreator::Grid(Vector3f(20, 20, 0), Color(0.5f,0.5f,0.5f));
    grid->Matrix.Translation(-10, -10, 0);
    grid->Name("Grid");
    scene->AddChild(grid);

    // ajout du light effect de base
    scene->AddChild(_lightingEffect);

    _graphic->GetSceneManager()->AddScene(scene);
    _scenes.push_back(scene);
    if (_GUI != NULL)
        _graphic->GetSceneManager()->BringToFront(_GUI);
    SelectScene(scene);

    if (_emitChanges)
    {
        emit SignalScenesChanged(&Scenes());
        emit SignalCurrentSceneChanged(_currentScene);
    }
}

void EditorEngine::RemoveScene(Graphic::SceneGraph *scene)
{
    if (scene == NULL)
        throw Utils::Exception("EditorEngine::RemoveScene", "The given scene is null.");

    ListPScene::iterator it = find(_scenes.begin(), _scenes.end(), scene);
    if (it == _scenes.end())
        throw Utils::Exception("EditorEngine::RemoveScene", "Can't delete the scene witch not belong to the editor engine.");
    if (scene == _currentScene)
    {
        _currentScene = NULL;
        _currentNodeSelected = NULL;
    }
    if (scene == _GUI)
        _GUI = NULL;
    _graphic->GetSceneManager()->RemoveScene(scene);
    _scenes.erase(it);
    delete scene;
    if (_emitChanges)
    {
        emit SignalScenesChanged(&Scenes());
        emit SignalCurrentSceneChanged(_currentScene);
    }
}

void EditorEngine::SelectScene(Nc::Graphic::SceneGraph *scene)
{
    if (scene == NULL)
        throw Utils::Exception("EditorEngine::SelectScene", "The given scene is NULL.");

    // verifie que la scene fait bien partie de notre liste de scene
    ListPScene::iterator it = find(Scenes().begin(), Scenes().end(), scene);
    if (it == Scenes().end())
    {
        _graphic->GetSceneManager()->AddScene(scene);
        if (_GUI != NULL)
            _graphic->GetSceneManager()->BringToFront(_GUI);

    }
    _currentScene = scene;
    _currentNodeSelected = NULL;

    if (_emitChanges)
        emit SignalCurrentSceneChanged(scene);
}

void EditorEngine::LoadNode(const FileName &file)
{
    if (_currentScene == NULL)
        throw Utils::Exception("EditorEngine::LoadNode", " the current scene is not set");

    ISceneNode *n = _sceneNodeManager.Load(file);
    if (n == NULL)
        throw Utils::Exception("EditorEngine::LoadNode", "Failed to load the node " + file);
    n->Name(file.ShortFilename());

    Entity *entity = n->As<Entity>();
    if (entity != NULL)
        _currentScene->AddChild(entity);
    else
        throw Utils::Exception("EditorEngine::LoadNode", "Loading of a non entity node is not implemented yet.");
    if (_emitChanges)
        emit SignalSceneChanged(_currentScene);
}

void EditorEngine::SelectNodeCmd(Nc::Engine::IEvent *e)
{
    Graphic::ISceneNode *n = e->GetData<Graphic::ISceneNode*>();

    if (_currentNodeSelected != n)
    {
/*
        Graphic::Object *obj = _currentNodeSelected->AsWithoutThrow<Graphic::Object>();
        if (obj != NULL)
            obj->DisplayBox(false);

        obj = n->AsWithoutThrow<Graphic::Object>();
        if (obj != NULL)
            obj->DisplayBox(true);
*/
        _currentNodeSelected = n;
    }
}

void EditorEngine::RemoveNode(Graphic::ISceneNode *n)
{
    // si la node est une scene
    Graphic::SceneGraph *scene = n->AsWithoutThrow<Graphic::SceneGraph>();
    if (scene != NULL)
        RemoveScene(scene);
    else if (_currentScene != NULL)
    {
        // on remove la node mais uniquement dans la scene courante.
        // si l'object n'est pas dans la scene il ne sera pas delete

        // si la scene courant ne nous apartient pas, on ne delete pas
        ListPScene::iterator it = find(_scenes.begin(), _scenes.end(), _currentScene);
        if (it == _scenes.end())
            throw Utils::Exception("EditorEngine::RemoveScene", "Can't delete a node from a scene witch not belong to the editor engine.");

        if (_currentNodeSelected == n)
            _currentNodeSelected = NULL;
        if (_currentScene->CurrentCamera() == n)
            _currentScene->CurrentCamera(NULL);
        Entity *entity = n->AsWithoutThrow<Entity>();
        if (entity != NULL)
        {
            _currentScene->Lock();
            if (_currentScene->RemoveChildRecursif(entity))
                if (_emitChanges)
                    emit SignalSceneChanged(_currentScene);
            _currentScene->Unlock();
        }
        else
            throw Utils::Exception("EditorEngine::RemoveNode", "Removing a non entity node is not implemented yet.");
    }
}

void EditorEngine::MoveNode(MoveStruct &moveArg)
{
    if (moveArg.node == NULL)
        throw Utils::Exception("EditorEngine::MoveNode", "node is null.");
    if (moveArg.into == NULL)
        throw Utils::Exception("EditorEngine::MoveNode", "into is null.");
    if (moveArg.parent == NULL)
        throw Utils::Exception("EditorEngine::MoveNode", "parent is null.");
    moveArg.into->Move(moveArg.node, moveArg.at, moveArg.parent, moveArg.oldAt);
}

void EditorEngine::ChangeRasterMode(Graphic::RasterEffect::Pattern mode)
{
    if (_currentScene != NULL)
        _currentScene->GetRasterEffect().SetPattern(mode);
}

void EditorEngine::SceneDeletedCmd(Engine::IEvent *e)
{
    if (_currentScene == e->GetData<Graphic::SceneGraph*>())
        _currentScene = NULL;
    _currentNodeSelected = NULL;
}
