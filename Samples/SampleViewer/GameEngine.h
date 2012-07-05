

#ifndef SAMPLEVIEWER_GAMEENGINE_H_
#define SAMPLEVIEWER_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/GUI/GUI.h>
#include "MainMenu.h"
#include "SampleFactory.h"

namespace SampleViewer
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, SampleViewer::GameEngine);

        public:
            enum IndexCmd
            {
                StartSample = 0,
                SampleSelected
            };

        public:
            GameEngine(Nc::Engine::Manager *manager);
            virtual ~GameEngine();

            void            CreateWindow(Nc::Graphic::Window *win);
            virtual void    LoadContent();
            virtual void    ReleaseContent();

            void            StartSampleCmd(Nc::Engine::IEvent *e);
            void            SampleSelectedCmd(Nc::Engine::IEvent *e);

        protected:
            virtual void    Update(float runningTime);
            virtual void    ManageWindowEvent(Nc::System::Event &event);
            virtual void    KeyboardEvent(Nc::System::Event &event);
            virtual void    MouseButtonEvent(Nc::System::Event &event);
            virtual void    MouseMotionEvent(Nc::System::Event &event);

            Nc::GUI::SceneGraph		*_scene;
            Nc::GUI::Console		*_console;
            MainMenu                *_menu;
            SampleFactory           *_sampleFactory;
            std::string             _currentSample;
    };
}

#endif
