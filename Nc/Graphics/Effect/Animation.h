
/*-----------------------------------------------------------------------------

	3dNovac Graphics
	Copyright (C) 2010-2011, The 3dNovac Team

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

    File Created At:        25/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Abstract template Class Animation, herite of Effect
    Help to manage an animation

    To use this class you have to implemente
    a new Animation class and a new Frame class by heritance

-----------------------------------------------------------------------------*/


#ifndef NOVAC_GRAPHIC_EFFECT_ANIMATION_H_
#define NOVAC_GRAPHIC_EFFECT_ANIMATION_H_

#include <vector>
#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        enum Pattern
        {
            Loop = 0,       /// return to the first frame at the end of the animation and continu
            Infinite,       /// The anim continu at the end with the last frame
            DieAtEnd        /// kill(_alive = false) the anim at the end of the animation, and display nothing
        };

        struct LGRAPHICS AnimationFrame
        {
            enum Pattern
            {
                Nop = 0,
                WaitSignal          /// wait a signal or wait the delay to pass the next frame
            };

            AnimationFrame(Pattern p, double d) : pattern(p), delay(d), signaled(false)     {}
            virtual ~AnimationFrame()                                                       {}

            virtual void Update(float runningTime) = 0;
            virtual void Render(ISceneGraph *scene) = 0;

            Pattern     pattern;        /// the pattern of the frame
            double      delay;          /// duration of the frame. If it's < 0, the delay is infinite
            bool        signaled;       /// set the signal at true to tell that the animation has received a signal
        };

        template<typename T /*= AnimationFrame*/>
        class Animation : public Effect
        {
            public:
                typedef std::list<T>    ListFrame;

            public:
                Animation(Pattern pattern = Loop)
                    : Effect(), _pattern(pattern), _totalDelay(0)   {_isPlaying = false; _indexCurrentFrame = 0;}
                virtual ~Animation()                                {}

                virtual Effect  *Clone() const                      {return new Animation<T>(*this);}

                Animation(const Animation &a)                       {Copy(a);}
                Animation &operator == (const Animation &a)         {Copy(a); return *this;}
                virtual void Copy(const Animation &a);

                ListFrame   &GetListFrame()                     {return _listFrames;}
                void        PushBack(const T &frame)            {_totalDelay += frame.delay; _listFrames.push_back(frame);}

                virtual void Reset()                            {_itCurrentFrame = _listFrames.begin(); _indexCurrentFrame = 0; _isPlaying = false;}
                virtual void Start();                                                   /// Start the animation
                virtual void Stop()                             {_isPlaying = false;}   /// Stop the animation
                virtual bool Started() const                    {return _isPlaying && _alive;}
                const T      &CurrentFrame()                    {return *_itCurrentFrame;}

                /// signal the current frame
                virtual void Signal(unsigned int i = 0);

                virtual void Update(float runningTime);         /// manage the pattern and call the virtual function `RenderFrame`
                virtual void Render(ISceneGraph *scene);        /// call the virtual function `DisplayFrame`

            protected:
                virtual void UpdateFrame(float runningTime)     {_itCurrentFrame->Update(runningTime);}
                virtual void RenderFrame(ISceneGraph *scene)
                {
                    bool pushed = false;
                    if (!Matrix.IsIdentity())
                    {
                        pushed = true;
                        scene->PushModelMatrix();
                        scene->ModelMatrix().AddTransformation(Matrix);
                    }
                    _itCurrentFrame->Render(scene);
                    if (pushed)
                        scene->PopModelMatrix();
                }

                Pattern         _pattern;       /// define the pattern of the animation
                bool            _isPlaying;     /// if this statement is false, no Update and no Display
                double          _totalDelay;    /// the total delay of the animations (all frames)

                unsigned int                    _indexCurrentFrame;
                typename ListFrame::iterator    _itCurrentFrame;
                ListFrame                       _listFrames;
                Utils::Clock                    _clock;
                double                          _lastTime;
        };

        template<typename T>
        void Animation<T>::Copy(const Animation &a)
        {
            _pattern = a._pattern;
            _isPlaying = a._isPlaying;
            _totalDelay = a._totalDelay;
            _listFrames = a._listFrames;
            _itCurrentFrame = _listFrames.begin();
            _indexCurrentFrame = 0;
            _clock = a._clock;
            _lastTime = a._lastTime;
        }

        template<typename T>
        void Animation<T>::Start()
        {
            _lastTime = 0;
            _isPlaying = true;
            _alive = true; // on reanime la particule :)
            _itCurrentFrame = _listFrames.begin();
            _indexCurrentFrame = 0;
            _clock.Reset();
        }

        template<typename T>
        void Animation<T>::Update(float runningTime)
        {
            if (_isPlaying && _alive && _itCurrentFrame != _listFrames.end())
            {
                // if it's time to change the frame, go to the good frame
                for (bool signaled = false; (_itCurrentFrame->delay >= 0 && _clock.ElapsedTime() - _lastTime > _itCurrentFrame->delay) ||
                                            ((signaled = _itCurrentFrame->pattern == AnimationFrame::WaitSignal && _itCurrentFrame->signaled));
                    _lastTime = _clock.ElapsedTime())
                {
                    if (signaled)
                        _itCurrentFrame->signaled = false;
                    if (_indexCurrentFrame >= _listFrames.size() - 1)
                    {
                        if (_pattern == DieAtEnd)
                            _alive = false;
                        else if (_pattern == Loop)
                        {
                            _itCurrentFrame = _listFrames.begin();
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
            else if (_itCurrentFrame != _listFrames.end())
                _isPlaying = false;
        }

        template<typename T>
        void Animation<T>::Render(ISceneGraph *scene)
        {
            if (Started() && _itCurrentFrame != _listFrames.end())
                RenderFrame(scene);
        }

        template<typename T>
        void Animation<T>::Signal(unsigned int nb)
        {
            typename ListFrame::iterator it = _itCurrentFrame;
            for (unsigned int i = 0; i < nb && it != _listFrames.end(); ++i, ++it)
                it->signaled = true;
        }

    }
}

#endif
