
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "SpriteAnim.h"

using namespace Nc::Graphic;

SpriteAnimFrame::SpriteAnimFrame(ParentType::Pattern p, double d)
    : ParentType(p, d), sprite(NULL)
{
}

SpriteAnimFrame::SpriteAnimFrame(ParentType::Pattern p, double d, Sprite *s)
    : ParentType(p, d), sprite(s)
{
}

SpriteAnimFrame::SpriteAnimFrame(const SpriteAnimFrame &f)
    : ParentType(f)
{
    sprite = (f.sprite) ? new Sprite(*f.sprite) : NULL;
}

SpriteAnimFrame::~SpriteAnimFrame()
{
    if (sprite)
        delete sprite;
}

SpriteAnimFrame &SpriteAnimFrame::operator = (const SpriteAnimFrame &f)
{
    ParentType::operator=(f);
    if (sprite)
        delete sprite;
    sprite = (f.sprite) ? new Sprite(*f.sprite) : NULL;
    return *this;
}

void SpriteAnimFrame::Update(float runningTime)
{
}

void SpriteAnimFrame::Render(SceneGraph *scene)
{
    if (sprite)
        sprite->RenderNode(scene);
}
