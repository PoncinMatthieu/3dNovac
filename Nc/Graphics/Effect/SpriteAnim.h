
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

#ifndef NC_GRAPHICS_2D_SPRITE_ANIM_H_
#define NC_GRAPHICS_2D_SPRITE_ANIM_H_

#include "FramesAnimation.h"
#include "../Object/Sprite.h"

namespace Nc
{
    namespace Graphic
    {
        /// Provide a Sprite Animation Frame to render and manage an animation of sprites `SpriteAnim`
        /**
            It's simply a container of sprite, a frame is composed of one sprite, so the Animation class will animate his collection of frames.
        */
        class LGRAPHICS SpriteAnimFrame : public FramesAnimation<SpriteAnimFrame>::Frame
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(FramesAnimation<SpriteAnimFrame>::Frame, System::Object, Nc::Graphic::SpriteAnimFrame)

            public:
                SpriteAnimFrame(Parent::Pattern p, double d)                : Parent(p, d), sprite(NULL)                    {}
                SpriteAnimFrame(Parent::Pattern p, double d, Sprite *s)     : Parent(p, d), sprite(s)                       {}
                SpriteAnimFrame(const SpriteAnimFrame &f)                   : Parent(f), sprite(new Sprite(*f.sprite))      {}
                virtual ~SpriteAnimFrame()                                  {delete sprite;}

                virtual ISceneNode      *Clone() const                      {return new SpriteAnimFrame(*this);}

                SpriteAnimFrame &operator = (const SpriteAnimFrame &f)
                {
                    Parent::operator=(f);
                    if (sprite)
                        delete sprite;
                    sprite = new Sprite(*f.sprite);
                    return *this;
                }

            protected:
                /** Update the sprite */
                virtual void Update(float runningTime);
                /** Render the sprite */
                virtual void Render(SceneGraph *scene);

            public:
                Sprite      *sprite;        ///< the sprite of the frame
        };
    }
}

#endif
