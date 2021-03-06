
#ifndef TUTO1_GAMEENGINE_H_
#define TUTO1_GAMEENGINE_H_

#include <Nc/Core/Engine/MainEngine.h>
#include <Nc/Graphics/Graphics.h>

class GameEngine : public Nc::Engine::MainEngine
{
    public:
        GameEngine(Nc::Graphic::Window *window, Nc::Engine::Manager *manager);
        virtual ~GameEngine();

        protected:
        virtual void	LoadContent();
        virtual void	ReleaseContent();

	// will be called in the engine's loop to update your scene.
        virtual void	Update(float runningTime);

        // manage the OpenGL context.
        virtual void	CreateContext()		{_context = _window->CreateSharedContext();}
        virtual void	ActiveContext()		{_context->Active();}
        virtual void	DisableContext()	{_context->Disable();}

        // manage window inputs.
        virtual void	ManageWindowEvent(Nc::System::Event &event);
        virtual void	KeyboardEvent(Nc::System::Event &event);
        virtual void	MouseButtonEvent(Nc::System::Event &event);
        virtual void	MouseMotionEvent(Nc::System::Event &event);

    private:
        Nc::Graphic::IWindow		*_window;	// the window used to render everything.
        Nc::Graphic::GLContext		*_context;	// the OpenGL context, needed to use the graphic engine.
        Nc::Graphic::SceneGraph		*_scene3d;	// our scene 3d, displayed by the graphic engine.
        Nc::Graphic::StandardCamera3d	*_camera;	// our camera used by our scene. We use it to resize and update it with the window inputs.
};

#endif
