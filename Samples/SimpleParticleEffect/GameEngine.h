

#ifndef POPULOUS_GAME_ENGINE
#define POPULOUS_GAME_ENGINE

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/GUI/GUI.h>
#include "ParticleEffect.h"

class GameEngine : public Nc::Contrib::GameEngine
{
    public:
        GameEngine(Nc::Graphic::Engine *graphic, Nc::Engine::Manager *manager);
        virtual ~GameEngine();

        void            CreateWindow(Nc::Graphic::Window *win);
        virtual void    LoadContent();
        virtual void    ReleaseContent();

    protected:
        virtual void    Update(float runningTime);
        virtual void    ManageWindowEvent(Nc::System::Event &event);
        virtual void    KeyboardEvent(Nc::System::Event &event);
        virtual void    MouseButtonEvent(Nc::System::Event &event);
        virtual void    MouseMotionEvent(Nc::System::Event &event);

	void		CreateGUI();

        Nc::Graphic::SceneGraph			*_scene;
        Nc::GUI::SceneGraph			    *_sceneGUI;
        Nc::Graphic::StandardCamera3d   *_camera;

        ParticleEffect				    *_particleEffect;
};

#endif
