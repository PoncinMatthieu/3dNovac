

#ifndef SIMPLEPARTICLEEFFECT_GAMEENGINE_H_
#define SIMPLEPARTICLEEFFECT_GAMEENGINE_H_

#include <Nc/Contrib/GameEngine.h>
#include <Nc/Graphics/Camera/StandardCamera3d.h>
#include <Nc/GUI/GUI.h>
#include "ParticleEffect.h"

namespace SimpleParticleEffect
{
    class GameEngine : public Nc::Contrib::GameEngine
    {
        public:
            NC_SYSTEM_DEFINE_OBJECT(Nc::Contrib::GameEngine, SimpleParticleEffect::GameEngine);

        public:
            GameEngine(Nc::Graphic::IWindow *window, Nc::Engine::Manager *manager);
            virtual ~GameEngine();

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
            Nc::Graphic::StandardCamera3d   *_camera;

            ParticleEffect				    *_particleEffect;
    };
}

#endif
