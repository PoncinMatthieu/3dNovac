
#include "SampleFactory.h"
#include "../bSplinesDemo/GameEngine.h"

using namespace SampleViewer;

SampleFactory::SampleFactory(Nc::Graphic::Engine *graphicEngine, Nc::Engine::Manager *manager)
    : _graphicEngine(graphicEngine), _manager(manager)
{
    _sampleNames.push_back("bSplinesDemo");

    _sampleNames.push_back("test1");
    _sampleNames.push_back("test2");
    _sampleNames.push_back("test3");
    _sampleNames.push_back("test4");
}

Nc::Contrib::GameEngine     *SampleFactory::CreateSample(const std::string &name)
{
    if (name == "bSplinesDemo")
        return new bSplinesDemo::GameEngine(_manager, _graphicEngine);


    return NULL;
}
