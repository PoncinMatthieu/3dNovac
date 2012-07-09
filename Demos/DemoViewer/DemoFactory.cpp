
#include "DemoFactory.h"
#include "../bSplinesDemo/GameEngine.h"
#include "../FBEffectsDemo/GameEngine.h"
#include "../FrustumDemo/GameEngine.h"
#include "../GUIDemo/GameEngine.h"
#include "../LightDemo/GameEngine.h"
#include "../SimpleParticleEffect/GameEngine.h"
#include "../SokobanDemo/GameEngine.h"

using namespace Nc;
using namespace DemoViewer;

DemoFactory::DemoFactory(Nc::Engine::Manager *manager)
    : _manager(manager)
{
    _demoNames.push_back("SokobanDemo");
    _demoNames.push_back("FBEffectsDemo");
    _demoNames.push_back("FrustumDemo");
    _demoNames.push_back("LightDemo");
    _demoNames.push_back("SimpleParticleEffect");
//    _demoNames.push_back("GUIDemo");
    _demoNames.push_back("bSplinesDemo");
}

Nc::Contrib::GameEngine     *DemoFactory::CreateDemo(Graphic::IWindow *window, const std::string &name)
{
    Nc::Contrib::GameEngine *engine = NULL;
    if (name == "bSplinesDemo")
        engine = new bSplinesDemo::GameEngine(window, _manager);
    else if (name == "FBEffectsDemo")
        engine = new FBEffectsDemo::GameEngine(window, &_sceneNodeFormatManager, _manager);
    else if (name == "FrustumDemo")
        engine = new FrustumDemo::GameEngine(window, &_sceneNodeFormatManager, _manager);
//    else if (name == "GUIDemo")
//        engine = new GUIDemo::GameEngine(window, _manager);
    else if (name == "LightDemo")
        engine = new LightDemo::GameEngine(window, &_sceneNodeFormatManager, _manager);
    else if (name == "SimpleParticleEffect")
        engine = new SimpleParticleEffect::GameEngine(window, _manager);
    else if (name == "SokobanDemo")
        engine = new SokobanDemo::GameEngine(window, &_sceneNodeFormatManager, _manager);
    return engine;
}
