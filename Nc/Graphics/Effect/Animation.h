
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

#ifndef NOVAC_GRAPHIC_EFFECT_ANIMATION_H_
#define NOVAC_GRAPHIC_EFFECT_ANIMATION_H_

#include <vector>
#include "Effect.h"

namespace Nc
{
    namespace Graphic
    {
        /// Abstract class used to define a frame in an `Animation` process
        /**
            Inherite of it to create your own animation
            It's possible to send a signal to a frame, call the function Animation::Signal() to send a signal to the current frame
        */
        struct LGRAPHICS AnimationFrame
        {
            /// The pattern of the frame
            enum Pattern
            {
                Nop = 0,
                WaitSignal              ///< wait a signal or wait the delay to pass the next frame
            };

            AnimationFrame(Pattern p, double d) : pattern(p), delay(d), signaled(false)     {}
            virtual ~AnimationFrame()                                                       {}

            /** Update the frame */
            virtual void Update(float runningTime) = 0;
            /** Render the frame */
            virtual void Render(ISceneGraph *scene) = 0;

            Pattern     pattern;        ///< the pattern of the frame
            double      delay;          ///< duration of the frame. If it's < 0, the delay is infinite
            bool        signaled;       ///< set the signal at true to tell that the animation has received a signal
        };

        /// Enum to define the comportement of an Animation
        enum AnimationPattern
        {
            Loop = 0,       ///< return to the first frame at the end of the animation and continu
            Infinite,       ///< The anim continu at the end with the last frame
            DieAtEnd        ///< kill(_alive = false) the anim at the end of the animation, and display nothing
        };

        /// Template class animation to manage any animation
        /**
            the typename T should be the type of AnimationFrame. An animation is typically composed by a list of animation frame <br/>
            So to use this class you will have to implemente a new Frame class by heritance like the SpriteAnimFrame class
        */
        template<typename T>
        class Animation : public Effect
        {
            public:
                typedef std::list<T>    ListFrame;  ///< The list of frame

            public:
                Animation(AnimationPattern pattern = Loop)
                    : Effect(), _pattern(pattern), _totalDelay(0)   {_isPlaying = false; _indexCurrentFrame = 0;}
                virtual ~Animation()                                {}

                Animation(const Animation &a)                       {Copy(a);}
                Animation &operator == (const Animation &a)         {Copy(a); return *this;}
                virtual void Copy(const Animation &a);

                /** Create and Copy a new animation */
                virtual Effect  *Clone() const                      {return new Animation<T>(*this);}

                /** Return the frame list */
                ListFrame   &GetListFrame()                     {return _listFrames;}
                /** Push a new frame */
                void        PushBack(const T &frame)            {_totalDelay += frame.delay; _listFrames.push_back(frame);}

                /** Recompute the total delay of the animation, could be invalid if a frame as an Infinite delay (<0) */
                void        ResetTotalDelay();

                /** Reset the animation, return to the first frame and stop the anim */
                virtual void Reset()                            {_itCurrentFrame = _listFrames.begin(); _indexCurrentFrame = 0; _isPlaying = false;}
                /** Start the anim */
                virtual void Start();                                                   ///< Start the animation
                /** Stop the anim */
                virtual void Stop()                             {_isPlaying = false;}   ///< Stop the animation
                /** true if the anim is started */
                virtual bool Started() const                    {return _isPlaying && _alive;}
                /** Return the current frame */
                const T      &CurrentFrame()                    {return *_itCurrentFrame;}

                /** Send a signal to the current frame */
                virtual void Signal(unsigned int i = 0);

                /** Update the animation, manage the pattern and call the virtual function `UpdateFrame` */
                virtual void Update(float runningTime);
                /** Render the current frame, so call the virtual function `RenderFrame` */
                virtual void Render(ISceneGraph *scene);

            protected:
                /** Update the current frame */
                virtual void UpdateFrame(float runningTime)     {_itCurrentFrame->Update(runningTime);}
                /** Render the current frame */
                virtual void RenderFrame(ISceneGraph *scene);

                AnimationPattern                _pattern;       ///< define the pattern of the animation
                bool                            _isPlaying;     ///< if this statement is false, no Update and no Display
                double                          _totalDelay;    ///< the total delay of the animations (all frames)

                unsigned int                    _indexCurrentFrame;     ///< the index of the current frame
                typename ListFrame::iterator    _itCurrentFrame;        ///< the iterator if the current frame
                ListFrame                       _listFrames;            ///< the list of frames
                Utils::Clock                    _clock;                 ///< the clock to manage time between two frames
                double                          _lastTime;              ///< the last time of the last frame
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

        template<typename T>
        void Animation<T>::ResetTotalDelay()
        {
            _totalDelay = 0;
            for (typename ListFrame::iterator it = _listFrames.begin(); it != _listFrames.end(); ++it)
                _totalDelay += it->delay;
        }

        template<typename T>
        void Animation<T>::RenderFrame(ISceneGraph *scene)
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
    }
}

#endif
