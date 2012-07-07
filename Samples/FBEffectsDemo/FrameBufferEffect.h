
#ifndef FBEFFECTSDEMO_FRAMEBUFFEREFFECT_H_
#define FBEFFECTSDEMO_FRAMEBUFFEREFFECT_H_

#include <Nc/Graphics/GL/FrameBuffer.h>
#include <Nc/Graphics/Effect/Effect.h>
#include <Nc/Graphics/Camera/Camera2d.h>
#include <Nc/Graphics/Object/Sprite.h>
#include <Nc/Graphics/Material/ProgrammableMaterial.h>

namespace FBEffectsDemo
{
    class FrameBufferEffect : public Nc::Graphic::Effect
      {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Nc::Graphic::Effect, Nc::System::Object, FBEffectDemo::FrameBufferEffect);

            public:
                FrameBufferEffect(Nc::Graphic::Window *window);
                ~FrameBufferEffect();

                virtual ISceneNode      *Clone() const		{return new FrameBufferEffect(*this);}

                void			        TriggerPostProcessActivation()	{_postProcessActivated = (_postProcessActivated) ? false : true;}

                void                    Resized()           {_fboNeedInit = true;}

            protected:
                virtual void            Render(Nc::Graphic::SceneGraph *scene);
                virtual void		    Update(float elapsedTime);

            private:
                void			InitFbo1();

            private:
                Nc::Graphic::Window                 *_window;
                bool                                _fboNeedInit;

                Nc::Graphic::GL::FrameBuffer        _fboPass1;
                Nc::Graphic::Camera2d		        *_camera2d;
                Nc::Graphic::Sprite			        *_sprite;

                bool			                    _postProcessActivated;

                Nc::Graphic::ProgrammableMaterial   *_postProcessMaterial;
                Nc::Graphic::GL::Uniform<float,2>   *_sketchJitter;
                float					            _sketchJitterSpeed;
                float					            _timeToNextJitter;
      };
}

#endif
