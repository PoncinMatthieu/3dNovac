
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, 3dNovac Team

    This file is part of 3dNovac.

    3dNovac is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    3dNovac is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with 3dNovac.  If not, see <http://www.gnu.org/licenses/>.

    File Created At:        04/06/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/


#ifndef NC_GRAPHICS_EFFECT_FRAMESANIMATION_H_
#define NC_GRAPHICS_EFFECT_FRAMESANIMATION_H_

#include "Animation.h"

namespace Nc
{
    namespace Graphic
    {
        /// Template class animation to manage any animation
        /**
            the typename T should be the type of AnimationFrame. An animation is typically composed by a list of animation frame <br/>
            So to use this class you will have to implemente a new Frame class by heritance like the SpriteAnimFrame class
        */
        template<typename T>
        class FramesAnimation : public Animation
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Animation);
                NC_UTILS_DEFINE_VISITABLE(System::Object);
                static const char     *ClassName()                  {return _uniqueClassName.c_str();}
                virtual const char    *ResolvedClassName() const    {return ClassName();}

            public:
                typedef std::list<T>                        ListFrame;

                /// Enum to define the comportement of an Animation
                enum Pattern
                {
                    Loop = 0,       ///< return to the first frame at the end of the animation and continu
                    Infinite,       ///< The anim continu at the end with the last frame
                    DieAtEnd        ///< kill (_alive = false) the anim at the end of the animation, and display nothing
                };

                /// Abstract class used to define a frame in an `Animation` process
                /**
                    Inherite of it to create your own animation
                    It's possible to send a signal to a frame, call the function Animation::Signal() to send a signal to the current frame
                */
                class LGRAPHICS Frame : public Effect
                {
                    public:
                        NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Effect, System::Object, Nc::Graphic::Frame);

                        /// The pattern of the frame
                        enum Pattern
                        {
                            Nop = 0,
                            WaitSignal              ///< wait a signal or wait the delay to pass the next frame
                        };

                    public:
                        Frame(Pattern p, double d)
                            : Effect(), pattern(p), delay(d), signaled(false)           {}
                        virtual ~Frame()                           {}

                        virtual ISceneNode      *Clone() const = 0;

                    protected:
                        /** Update the frame */
                        virtual void            Update(float runningTime) = 0;
                        /** Render the frame */
                        virtual void            Render(SceneGraph *scene) = 0;

                    public:
                        Pattern             pattern;        ///< the pattern of the frame
                        double              delay;          ///< duration of the frame. If it's < 0, the delay is infinite
                        bool                signaled;       ///< set the signal at true to tell that the animation has received a signal
                };

            public:
                FramesAnimation(Pattern pattern = Loop)
                    : Animation(), _pattern(pattern), _isPlaying(false), _totalDelay(0), _indexCurrentFrame(0)
                    {}
                virtual ~FramesAnimation()                                  {}

                FramesAnimation(const FramesAnimation &a) : Animation(a)    {Copy(a);}
                FramesAnimation &operator == (const FramesAnimation &a)     {Copy(a); return *this;}
                virtual void Copy(const FramesAnimation &a);

                virtual ISceneNode      *Clone() const                      {return new FramesAnimation(*this);}

                /** \return the frame list */
                inline ListFrame        &GetListFrame()                     {return _frames;}
                /** \return the frame list */
                inline const ListFrame  &GetListFrame() const               {return _frames;}
                /** Push a new frame */
                void                    PushBack(const T &frame)            {_totalDelay += frame.delay; GetListFrame().push_back(frame);}

                /** Recompute the total delay of the animation, could be invalid if a frame as an Infinite delay (<0) */
                void        ResetTotalDelay();

                /** Reset the animation, return to the first frame and stop the anim */
                virtual void Reset();
                /** Start the anim */
                virtual void Start();                                                   ///< Start the animation
                /** Stop the anim */
                virtual void Stop()                                         {_isPlaying = false;}   ///< Stop the animation
                /** true if the anim is started */
                virtual bool Started() const                                {return _isPlaying && _alive;}
                /** \return the current frame */
                const T      &CurrentFrame()                                {return *_itCurrentFrame;}

                /** Send a signal to the current frame */
                virtual void Signal(unsigned int i = 0);

            protected:
                /** Update the animation, manage the pattern and call the virtual function `UpdateFrame` */
                virtual void Update(float runningTime);
                /** Render the current frame, so call the virtual function `RenderFrame` */
                virtual void Render(SceneGraph *scene);

                /** Update the current frame */
                virtual void UpdateFrame(float runningTime)                 {_itCurrentFrame->UpdateNode(runningTime);}
                /** Render the current frame */
                virtual void RenderFrame(SceneGraph *scene);

            protected:
                Pattern                         _pattern;               ///< define the pattern of the animation
                bool                            _isPlaying;             ///< if this statement is false, no Update and no Display
                double                          _totalDelay;            ///< the total delay of the animations (all frames)

                unsigned int                    _indexCurrentFrame;     ///< the index of the current frame
                typename ListFrame::iterator    _itCurrentFrame;        ///< the iterator if the current frame
                ListFrame                       _frames;                ///< the frames of the animation
                Utils::Clock                    _clock;                 ///< the clock to manage time between two frames
                double                          _lastTime;              ///< the last time of the last frame

            private:
                static const std::string        _uniqueClassName;     ///< the static class name used to has a unique class name between the diferent templates.
        };
    }
}

#include "../Scene/SceneGraph.h"

namespace Nc
{
    namespace Graphic
    {
        template<typename T>
        const std::string       FramesAnimation<T>::_uniqueClassName = std::string("Nc::Graphic::FramesAnimation<") + std::string(T::ClassName()) + std::string(">");

        template<typename T>
        void FramesAnimation<T>::Copy(const FramesAnimation &a)
        {
            _pattern = a._pattern;
            _isPlaying = a._isPlaying;
            _totalDelay = a._totalDelay;
            _frames = a._frames;
            _itCurrentFrame = GetListFrame().begin();
            _indexCurrentFrame = 0;
            _clock = a._clock;
            _lastTime = a._lastTime;
            Matrix = a.Matrix;
        }

        template<typename T>
        void FramesAnimation<T>::Start()
        {
            _lastTime = 0;
            _isPlaying = true;
            _alive = true; // on reanime la particule :)
            _itCurrentFrame = GetListFrame().begin();
            _indexCurrentFrame = 0;
            _clock.Reset();
        }

        template<typename T>
        void FramesAnimation<T>::Reset()
        {
            _itCurrentFrame = GetListFrame().begin();
            _indexCurrentFrame = 0;
            _isPlaying = false;
        }

        template<typename T>
        void FramesAnimation<T>::Update(float runningTime)
        {
            if (!_activated)
                return;

            if (_isPlaying && _alive && _itCurrentFrame != GetListFrame().end())
            {
                // if it's time to change the frame, go to the good frame
                for (bool signaled = false; (_itCurrentFrame->delay >= 0 && _clock.ElapsedTime() - _lastTime > _itCurrentFrame->delay) ||
                                            ((signaled = _itCurrentFrame->pattern == Frame::WaitSignal && _itCurrentFrame->signaled));
                    _lastTime = _clock.ElapsedTime())
                {
                    if (signaled)
                        _itCurrentFrame->signaled = false;
                    if (_indexCurrentFrame >= GetListFrame().size() - 1)
                    {
                        if (_pattern == DieAtEnd)
                            _alive = false;
                        else if (_pattern == Loop)
                        {
                            _itCurrentFrame = GetListFrame().begin();
                            _indexCurrentFrame = 0;
                        }
                    }
                    else
                    {
                        ++_itCurrentFrame;
                        ++_indexCurrentFrame;
                    }
                }
                UpdateFrame(runningTime);
            }
            else if (_itCurrentFrame != GetListFrame().end())
                _isPlaying = false;
        }

        template<typename T>
        void FramesAnimation<T>::Render(SceneGraph *scene)
        {
            if (!_alive || !Started() || _itCurrentFrame == GetListFrame().end())
                return;
            RenderFrame(scene);
        }

        template<typename T>
        void FramesAnimation<T>::Signal(unsigned int nb)
        {
            typename ListFrame::iterator it = _itCurrentFrame;
            for (unsigned int i = 0; i < nb && it != GetListFrame().end(); ++i, ++it)
                it->signaled = true;
        }

        template<typename T>
        void FramesAnimation<T>::ResetTotalDelay()
        {
            _totalDelay = 0;
            for (typename ListFrame::iterator it = GetListFrame().begin(); it != GetListFrame().end(); ++it)
                _totalDelay += it->delay;
        }

        template<typename T>
        void FramesAnimation<T>::RenderFrame(SceneGraph *scene)
        {
            bool pushed = false;
            if (!Matrix.IsIdentity())
            {
                pushed = true;
                scene->PushModelMatrix();
                scene->ModelMatrix().AddTransformation(Matrix);
            }
            _itCurrentFrame->RenderNode(scene);
            if (pushed)
                scene->PopModelMatrix();
        }
    }
}

#endif
