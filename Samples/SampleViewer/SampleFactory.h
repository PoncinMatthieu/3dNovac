
#ifndef SAMPLEVIEWER_SAMPLEFACTORY_H_
#define SAMPLEVIEWER_SAMPLEFACTORY_H_

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Engine.h>
#include <Nc/Contrib/GameEngine.h>
#include <Nc/GUI/WidgetSubWindow.h>


namespace SampleViewer
{
    class SampleFactory
    {
        public:
            SampleFactory(Nc::Graphic::Engine *graphicEngine, Nc::Engine::Manager *manager);

            inline const std::list<std::string>     &SampleNames() const                        {return _sampleNames;}

            Nc::Contrib::GameEngine                 *CreateSample(Nc::GUI::WidgetSubWindow *w, const std::string &name);

        private:
            Nc::Graphic::Engine         *_graphicEngine;
            Nc::Engine::Manager         *_manager;
            std::list<std::string>      _sampleNames;
    };
}

#endif
