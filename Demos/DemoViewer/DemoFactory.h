
#ifndef SAMPLEVIEWER_SAMPLEFACTORY_H_
#define SAMPLEVIEWER_SAMPLEFACTORY_H_

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include <Nc/Graphics/Scene/SceneNodeFormatManager.h>
#include <Nc/Contrib/GameEngine.h>

namespace DemoViewer
{
    class DemoFactory
    {
        public:
            DemoFactory(Nc::Engine::Manager *manager);

            inline const std::list<std::string>     &DemoNames() const                        {return _demoNames;}

            Nc::Contrib::GameEngine                 *CreateDemo(Nc::Graphic::IWindow *window, const std::string &name);

        private:
            Nc::Engine::Manager                     *_manager;
            std::list<std::string>                  _demoNames;
            Nc::Graphic::SceneNodeFormatManager	    _sceneNodeFormatManager;
    };
}

#endif
