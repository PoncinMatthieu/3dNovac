

#ifndef POPULOUS_GAME_ENGINE
#define POPULOUS_GAME_ENGINE

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
            enum IndexCmd
            {
                StartSample = 0
            };

        public:
            GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager);
            virtual ~GameEngine();

            void            CreateWindow(Nc::Graphic::Window *win);
            virtual void    LoadContent();
            virtual void    ReleaseContent();

            void            StartSampleCmd(Nc::Engine::IEvent *e);

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
    };
}

#endif