
#include "SampleFactory.h"
#include "../bSplinesDemo/GameEngine.h"

using namespace SampleViewer;

SampleFactory::SampleFactory(Nc::Graphic::Engine *graphicEngine, Nc::Engine::Manager *manager)
    : _graphicEngine(graphicEngine), _manager(manager)
{
    _sampleNames.push_back("bSplinesDemo");
}

Nc::Contrib::GameEngine     *SampleFactory::CreateSample(const std::string &name)
{
    Nc::Contrib::GameEngine *engine = NULL;
    if (name == "bSplinesDemo")
        engine = new bSplinesDemo::GameEngine(_manager, _graphicEngine);


    // init the engine
    if (engine != NULL)
    {
        engine->LimitFrameRate(30);
        LOG << "engine->Name() = " << engine->Name() << std::endl;
        _manager->AddEngine(engine->Name(), engine);
        engine->Start();

    }

    return engine;
}
