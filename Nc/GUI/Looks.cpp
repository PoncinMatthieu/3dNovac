
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

    File Created At:        16/04/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Graphics/Object/Sprite.h>
#include "Looks.h"
#include "WindowStyle.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

StripLook::StripLook(const std::string &name)
{
    _spriteLeft = WindowStyle::Instance().GetNewSprite(name + WindowStyle::LooksSpriteName::StripLeft);
    _spriteRight = WindowStyle::Instance().GetNewSprite(name + WindowStyle::LooksSpriteName::StripRight);
    _spriteMiddle = WindowStyle::Instance().GetNewSprite(name + WindowStyle::LooksSpriteName::StripMiddle);
}

StripLook::StripLook(const StripLook &l)
{
    Copy(l);
}

StripLook &StripLook::operator = (const StripLook &l)
{
    Copy(l);
    return *this;
}

StripLook::~StripLook()
{
    if (_spriteLeft != NULL)
        delete _spriteLeft;
    if (_spriteRight != NULL)
        delete _spriteRight;
    if (_spriteMiddle != NULL)
        delete _spriteMiddle;
}

void    StripLook::Copy(const StripLook &l)
{
    _spriteLeft = l._spriteLeft;
    _spriteRight = l._spriteRight;
    _spriteMiddle = l._spriteMiddle;

    if (_spriteLeft != NULL)
        _spriteLeft = (Sprite*)_spriteLeft->Clone();
    if (_spriteRight != NULL)
        _spriteRight = (Sprite*)_spriteRight->Clone();
    if (_spriteMiddle != NULL)
        _spriteMiddle = (Sprite*)_spriteMiddle->Clone();
}

void StripLook::Update(const Vector2i &size)
{
    if (_spriteLeft)
    {
        _spriteLeft->Size(Vector2f(_spriteLeft->Size()[0], size[1]));
    }
    if (_spriteMiddle)
    {
        int leftSize = (_spriteLeft) ? _spriteLeft->Size()[0] : 0;
        int rightSize = (_spriteRight) ? _spriteRight->Size()[0] : 0;

        _spriteMiddle->Size(Vector2f(size[0] - leftSize - rightSize, size[1]));
        _spriteMiddle->Matrix.Translation(leftSize, 0, 0);
    }
    if (_spriteRight)
    {
        int leftSize = (_spriteLeft) ? _spriteLeft->Size()[0] : 0;
        int middleSize = (_spriteMiddle) ? _spriteMiddle->Size()[0] : 0;

        _spriteRight->Size(Vector2f(_spriteRight->Size()[0], size[1]));
        _spriteRight->Matrix.Translation(leftSize + middleSize, 0, 0);
    }
}

void StripLook::Draw(Graphic::SceneGraph *scene)
{
    _spriteLeft->RenderNode(scene);
    _spriteMiddle->RenderNode(scene);
    _spriteRight->RenderNode(scene);
}


BoxLook::BoxLook(const std::string &name)
{
    // get the sprites from the window style (sprite name = name + position)
    _spriteLeftEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftEdge);
    _spriteTopEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetTopEdge);
    _spriteRightEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightEdge);
    _spriteBottomEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetBottomEdge);
    _spriteLeftTop = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftTop);
    _spriteRightTop = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightTop);
    _spriteLeftBottom = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftBottom);
    _spriteRightBottom = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightBottom);
    _spriteMiddle = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetMiddle);
}

BoxLook::BoxLook(const BoxLook &l)
{
    Copy(l);
}

BoxLook &BoxLook::operator = (const BoxLook &l)
{
    Copy(l);
    return *this;
}

BoxLook::~BoxLook()
{
    if (_spriteLeftEdge != NULL)
        delete _spriteLeftEdge;
    if (_spriteTopEdge != NULL)
        delete _spriteTopEdge;
    if (_spriteRightEdge != NULL)
        delete _spriteRightEdge;
    if (_spriteBottomEdge != NULL)
        delete _spriteBottomEdge;
    if (_spriteLeftTop != NULL)
        delete _spriteLeftTop;
    if (_spriteRightTop != NULL)
        delete _spriteRightTop;
    if (_spriteLeftBottom != NULL)
        delete _spriteLeftBottom;
    if (_spriteRightBottom != NULL)
        delete _spriteRightBottom;
    if (_spriteMiddle != NULL)
        delete _spriteMiddle;
}

void    BoxLook::Copy(const BoxLook &l)
{
    _spriteLeftEdge = l._spriteLeftEdge;
    _spriteTopEdge = l._spriteTopEdge;
    _spriteRightEdge = l._spriteRightEdge;
    _spriteBottomEdge = l._spriteBottomEdge;
    _spriteLeftTop = l._spriteLeftTop;
    _spriteRightTop = l._spriteRightTop;
    _spriteLeftBottom = l._spriteLeftBottom;
    _spriteRightBottom = l._spriteRightBottom;
    _spriteMiddle = l._spriteMiddle;

    if (_spriteLeftEdge != NULL)
        _spriteLeftEdge = (Sprite*)_spriteLeftEdge->Clone();
    if (_spriteTopEdge != NULL)
        _spriteTopEdge = (Sprite*)_spriteTopEdge->Clone();
    if (_spriteRightEdge != NULL)
        _spriteRightEdge = (Sprite*)_spriteRightEdge->Clone();
    if (_spriteBottomEdge != NULL)
        _spriteBottomEdge = (Sprite*)_spriteBottomEdge->Clone();
    if (_spriteLeftTop != NULL)
        _spriteLeftTop = (Sprite*)_spriteLeftTop->Clone();
    if (_spriteRightTop != NULL)
        _spriteRightTop = (Sprite*)_spriteRightTop->Clone();
    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom = (Sprite*)_spriteLeftBottom->Clone();
    if (_spriteRightBottom != NULL)
        _spriteRightBottom = (Sprite*)_spriteRightBottom->Clone();
    if (_spriteMiddle != NULL)
        _spriteMiddle = (Sprite*)_spriteMiddle->Clone();
}

void BoxLook::Update(const Vector2i &size)
{
    int lxLeftBottom = 0, lyLeftBottom = 0, lxLeftTop = 0, lyLeftTop = 0, lxRightBottom = 0, lyRightBottom = 0, lxRightTop = 0, lyRightTop = 0;

    // get the corner size
    if (_spriteLeftBottom != NULL)
    {
        lxLeftBottom = _spriteLeftBottom->TextureBox().Length(0);
        lyLeftBottom = _spriteLeftBottom->TextureBox().Length(1);
    }
    if (_spriteLeftTop != NULL)
    {
        lxLeftTop = _spriteLeftTop->TextureBox().Length(0);
        lyLeftTop = _spriteLeftTop->TextureBox().Length(1);
    }
    if (_spriteRightBottom != NULL)
    {
        lxRightBottom = _spriteRightBottom->TextureBox().Length(0);
        lyRightBottom = _spriteRightBottom->TextureBox().Length(1);
    }
    if (_spriteRightTop != NULL)
    {
        lxRightTop = _spriteRightTop->TextureBox().Length(0);
        lyRightTop = _spriteRightTop->TextureBox().Length(1);
    }

    // Set the size and position of the sprites
    if (_spriteMiddle != NULL)
    {
        _spriteMiddle->Size(Vector2f(size[0] - lxLeftBottom - lxLeftTop, size[1] - lyLeftBottom - lyLeftTop));
        _spriteMiddle->Matrix.Translation(lxLeftBottom, lyLeftBottom, 0);
    }

    if (_spriteLeftTop != NULL)
    {
        _spriteLeftTop->Size(Vector2f(_spriteLeftTop->TextureBox().Length(0), lyLeftTop));
        _spriteLeftTop->Matrix.Translation(0, size[1] - lyLeftTop, 0);
    }

    if (_spriteRightTop != NULL)
    {
        _spriteRightTop->Size(Vector2f(lxRightTop, lyRightTop));
        _spriteRightTop->Matrix.Translation(size[0] - lxRightTop, size[1] - lyRightTop, 0);
    }

    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom->Size(Vector2f(_spriteLeftBottom->TextureBox().Length(0), _spriteLeftBottom->TextureBox().Length(1)));

    if (_spriteRightBottom)
    {
        _spriteRightBottom->Size(Vector2f(lxRightBottom, lyRightBottom));
        _spriteRightBottom->Matrix.Translation(size[0] - lxRightBottom, 0, 0);
    }

    if (_spriteLeftEdge != NULL)
    {
        _spriteLeftEdge->Size(Vector2f(_spriteLeftEdge->TextureBox().Length(0), size[1] - lyLeftBottom - lyLeftTop));
        _spriteLeftEdge->Matrix.Translation(0, lyLeftBottom, 0);
    }

    if (_spriteTopEdge != NULL)
    {
        float s = _spriteTopEdge->TextureBox().Length(1);
        _spriteTopEdge->Size(Vector2f(size[0] - lxLeftTop - lxRightTop, s));
        _spriteTopEdge->Matrix.Translation(lxLeftTop, size[1] - s, 0);
    }

    if (_spriteRightEdge != NULL)
    {
        float s = _spriteRightEdge->TextureBox().Length(0);
        _spriteRightEdge->Size(Vector2f(s, size[1] - lyRightBottom - lyRightTop));
        _spriteRightEdge->Matrix.Translation(size[0] - s, lyRightBottom, 0);
    }

    if (_spriteBottomEdge != NULL)
    {
        _spriteBottomEdge->Size(Vector2f(size[0] - lxLeftBottom - lxRightBottom, _spriteBottomEdge->TextureBox().Length(1)));
        _spriteBottomEdge->Matrix.Translation(lxLeftBottom, 0, 0);
    }
}

void BoxLook::Draw(Graphic::SceneGraph *scene)
{
    scene->PushModelMatrix();

    // render middle sprite
    if (_spriteMiddle != NULL)
        _spriteMiddle->RenderNode(scene);

    // render corner sprites
    if (_spriteLeftTop != NULL)
        _spriteLeftTop->RenderNode(scene);
    if (_spriteRightTop != NULL)
        _spriteRightTop->RenderNode(scene);
    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom->RenderNode(scene);
    if (_spriteRightBottom != NULL)
        _spriteRightBottom->RenderNode(scene);

    // render edge sprites
    if (_spriteLeftEdge != NULL)
        _spriteLeftEdge->RenderNode(scene);
    if (_spriteTopEdge != NULL)
        _spriteTopEdge->RenderNode(scene);
    if (_spriteRightEdge != NULL)
        _spriteRightEdge->RenderNode(scene);
    if (_spriteBottomEdge != NULL)
        _spriteBottomEdge->RenderNode(scene);

    scene->PopModelMatrix();
}
