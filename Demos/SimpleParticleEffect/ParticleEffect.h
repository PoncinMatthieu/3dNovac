
#ifndef SIMPLEPARTICLEEFFECT_PARTICLEEFFECT_H_
#define SIMPLEPARTICLEEFFECT_PARTICLEEFFECT_H_

#include <Nc/Graphics/GL/VertexDescriptor.h>
#include <Nc/Graphics/Effect/Animation.h>
#include <Nc/Graphics/Material/ProgrammableMaterial.h>

namespace SimpleParticleEffect
{
    class ParticleEffect : public Nc::Graphic::Animation
    {
    private:
        NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Nc::Graphic::Animation, Nc::System::Object, ParticleEffect);

        struct VertexType
        {
            float	coord[3];
            float	speed[3];
            float	direction[3];
            float	acceleration;
            float	lifeTime;

            static const Nc::Graphic::GL::VertexDescriptor	&GetDescriptor();
            static unsigned int					GetDescriptorPriority()             {return sizeof(VertexType);}
        };

    public:
        ParticleEffect();
        ~ParticleEffect();

        virtual ISceneNode      *Clone() const		{return new ParticleEffect(*this);}

        virtual void            Start()             {_clock.Reset(); _started = true;}
        virtual void            Stop()              {_started = false;}
        virtual bool            Started() const     {return _started;}

        void                    Pause()             {_clock.Pause();}
        bool                    Paused()            {return _clock.Paused();}
        void                    Restart()           {_clock.Restart(); if (!_started) Start();}

    protected:
        virtual void            Render(Nc::Graphic::SceneGraph *scene);

        void			        RandomPointOnSphere(float point[3]);

    private:
        Nc::Graphic::ProgrammableMaterial	*_material;
        Nc::Graphic::GL::Drawable		    *_drawable;
        Nc::Utils::Clock			        _clock;

        bool                                _started;
    };
}

#endif
