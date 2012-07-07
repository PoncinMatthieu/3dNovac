
#ifndef SAMPLEVIEWER_SAMPLEFACTORY_H_
#define SAMPLEVIEWER_SAMPLEFACTORY_H_

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Contrib/GameEngine.h>

namespace SampleViewer
{
    class SampleFactory
    {
        public:
            SampleFactory(Nc::Engine::Manager *manager);

            inline const std::list<std::string>     &SampleNames() const                        {return _sampleNames;}

            Nc::Contrib::GameEngine                 *CreateSample(Nc::Graphic::Window *window, const std::string &name);

        private:
            Nc::Engine::Manager                     *_manager;
            std::list<std::string>                  _sampleNames;
            Nc::Graphic::SceneNodeFormatManager	    _sceneNodeFormatManager;
    };
}

#endif
