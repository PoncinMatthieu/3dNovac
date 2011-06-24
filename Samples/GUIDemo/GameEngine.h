

#ifndef POPULOUS_GAME_ENGINE
#define POPULOUS_GAME_ENGINE

#include <Nc/Graphics/Engine.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/GUI/GUI.h>

class GameEngine : public Nc::Engine::MainEngine
{
    public:
        GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager);
        virtual ~GameEngine();

        void            CreateWindow(Nc::Graphic::Window *win);
        virtual void    LoadContent();

    protected:
        virtual void    CreateContext()     {_context = _graphic->CreateSharedContext();}
        virtual void    ActiveContext()     {if (_context != NULL) _context->Active();}
        virtual void    DisableContext()    {if (_context != NULL) _context->Disable();}

        virtual void    Update(float runningTime);
        virtual void    ManageWindowEvent(Nc::System::Event &event);
        virtual void    KeyboardEvent(Nc::System::Event &event);
        virtual void    MouseButtonEvent(Nc::System::Event &event);
        virtual void    MouseMotionEvent(Nc::System::Event &event);

        Nc::Graphic::Engine             *_graphic;
        Nc::Graphic::GLContext          *_context;
        Nc::GUI::SceneGraph		*_sceneGUI;
	Nc::GUI::Console		*_console;
};

#endif
