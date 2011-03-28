
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

    Provide a Sprite Animation

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_2D_SPRITE_ANIM_H_
#define NC_GRAPHICS_2D_SPRITE_ANIM_H_

#include "Animation.h"
#include "../Object/2D/Sprite.h"

namespace Nc
{
    namespace Graphic
    {
        struct LGRAPHICS SpriteAnimFrame : public AnimationFrame
        {
            SpriteAnimFrame(AnimationFrame::Pattern p, double d) : AnimationFrame(p, d), sprite(NULL) {}
            SpriteAnimFrame(AnimationFrame::Pattern p, double d, Sprite *s) : AnimationFrame(p, d), sprite(s) {}
            SpriteAnimFrame(const SpriteAnimFrame &f) : AnimationFrame(f), sprite(new Sprite(*f.sprite))   {}
            virtual ~SpriteAnimFrame()      {delete sprite;}

            SpriteAnimFrame &operator = (const SpriteAnimFrame &f)
            {
                AnimationFrame::operator=(f);
                if (sprite)
                    delete sprite;
                sprite = new Sprite(*f.sprite);
                return *this;
            }

            virtual void Update(float runningTime);
            virtual void Render(ISceneGraph *scene);

            Sprite      *sprite;
        };

        typedef Animation<SpriteAnimFrame>  SpriteAnim;
    }
}

#endif
