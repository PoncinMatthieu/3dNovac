
#include "QSceneGraphManager.h"
#include "EditorEngine.h"
#include <Nc/Core/Engine/Manager.h>

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Editor;

QSceneGraphManager::QSceneGraphManager()
{
}

QSceneGraphManager::~QSceneGraphManager()
{
}

void    QSceneGraphManager::AddScene(Graphic::SceneGraph *scene)
{
    SceneGraphManager::AddScene(scene);
    if (_emitChanges)
        emit SignalScenesChanged(&_listScene);
}

void    QSceneGraphManager::RemoveScene(Graphic::SceneGraph *scene)
{
    Engine::Manager::PushEvent("Editor", EditorEngine::IndexSceneDeleted, scene);
    SceneGraphManager::RemoveScene(scene);
    if (_emitChanges)
        emit SignalScenesChanged(&_listScene);
}
