
#include <Nc/Graphics/Window/Window/SubWindow.h>
#include "SampleFactory.h"
#include "../bSplinesDemo/GameEngine.h"

using namespace Nc;
using namespace SampleViewer;

SampleFactory::SampleFactory(Nc::Engine::Manager *manager)
    : _manager(manager)
{
    _sampleNames.push_back("bSplinesDemo");
}

Nc::Contrib::GameEngine     *SampleFactory::CreateSample(Nc::GUI::WidgetSubWindow *w, const std::string &name)
{
    Nc::Contrib::GameEngine *engine = NULL;
    if (name == "bSplinesDemo")
        engine = new bSplinesDemo::GameEngine(_manager);


    // init the engine
    if (engine != NULL)
    {
        engine->LimitFrameRate(30);
        _manager->AddEngine(engine);

        // create a subwindow
//        Graphic::SubWindow *subWin = new Graphic::SubWindow(_graphicEngine->GetWindow());
//        subWin->Create(Vector2ui(800, 600));
//        engine->SetWindow(subWin);

        engine->SetWindow(w->SubWindow());

        // start the engine
        engine->Start();
    }

    return engine;
}
