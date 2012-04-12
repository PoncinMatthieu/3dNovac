
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

#include "SpriteWidget.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

SpriteWidget::SpriteWidget(Corner x, Corner y, const Vector2i &pos, const Vector2i &size)
    : Widget(x, y, pos, size)
{
}

SpriteWidget::~SpriteWidget()
{
}

void SpriteWidget::AddSprite(Sprite *s, const Vector2i &pos)
{
    _listSprite.push_back(SpritePos(s, pos));
    _stateChanged = true;
}

void SpriteWidget::Draw(Graphic::SceneGraph *scene)
{
    for (ListSpritePos::iterator it = _listSprite.begin(); it != _listSprite.end(); ++it)
    {
        scene->PushModelMatrix();
        scene->ModelMatrix().AddTranslation(it->pos.Data[0], it->pos.Data[1], 0.f);
        it->sprite->RenderNode(scene);
        scene->PopModelMatrix();
    }
}
