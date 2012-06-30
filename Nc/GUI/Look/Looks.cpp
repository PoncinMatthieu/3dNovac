
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

#include "Looks.h"
#include "StyleSheet.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

ILook::ILook()
    : color(1,1,1)
{
}

ILook::~ILook()
{
}

StripLook::StripLook(const std::string &name)
{
    StyleSheet &winStyle = StyleSheet::Instance();

    spriteLeft = winStyle.GetNewSprite(name + StyleSheet::LooksName::StripLeft);
    spriteRight = winStyle.GetNewSprite(name + StyleSheet::LooksName::StripRight);
    spriteMiddle = winStyle.GetNewSprite(name + StyleSheet::LooksName::StripMiddle);

    edges.left = (spriteLeft) ? spriteLeft->Size()[0] : 0;
    edges.right = (spriteRight) ? spriteRight->Size()[0] : 0;
    edges.top = 0;
    edges.bottom = 0;
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
    if (spriteLeft != NULL)
        delete spriteLeft;
    if (spriteRight != NULL)
        delete spriteRight;
    if (spriteMiddle != NULL)
        delete spriteMiddle;
}

void    StripLook::Copy(const StripLook &l)
{
    spriteLeft = l.spriteLeft;
    spriteRight = l.spriteRight;
    spriteMiddle = l.spriteMiddle;

    if (spriteLeft != NULL)
        spriteLeft = static_cast<Sprite*>(spriteLeft->Clone());
    if (spriteRight != NULL)
        spriteRight = static_cast<Sprite*>(spriteRight->Clone());
    if (spriteMiddle != NULL)
        spriteMiddle = static_cast<Sprite*>(spriteMiddle->Clone());
}

void StripLook::Update(const Vector2i &size)
{
    if (spriteLeft)
    {
        spriteLeft->Size(Vector2f(spriteLeft->Size()[0], size[1]));

        if (spriteLeft->GetColor() != color)
            spriteLeft->SetColor(color);
    }
    if (spriteMiddle)
    {
        int leftSize = (spriteLeft) ? spriteLeft->Size()[0] : 0;
        int rightSize = (spriteRight) ? spriteRight->Size()[0] : 0;

        spriteMiddle->Size(Vector2f(size[0] - leftSize - rightSize, size[1]));
        spriteMiddle->Matrix.Translation(leftSize, 0, 0);

        if (spriteMiddle->GetColor() != color)
            spriteMiddle->SetColor(color);
    }
    if (spriteRight)
    {
        int leftSize = (spriteLeft) ? spriteLeft->Size()[0] : 0;
        int middleSize = (spriteMiddle) ? spriteMiddle->Size()[0] : 0;

        spriteRight->Size(Vector2f(spriteRight->Size()[0], size[1]));
        spriteRight->Matrix.Translation(leftSize + middleSize, 0, 0);

        if (spriteRight->GetColor() != color)
            spriteRight->SetColor(color);
    }
}

void StripLook::Draw(Graphic::SceneGraph *scene)
{
    spriteLeft->RenderNode(scene);
    spriteMiddle->RenderNode(scene);
    spriteRight->RenderNode(scene);
}


BoxLook::BoxLook(const std::string &name)
{
    StyleSheet &winStyle = StyleSheet::Instance();

    spriteLeftEdge = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxLeftEdge);
    spriteTopEdge = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxTopEdge);
    spriteRightEdge = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxRightEdge);
    spriteBottomEdge = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxBottomEdge);
    spriteLeftTop = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxLeftTop);
    spriteRightTop = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxRightTop);
    spriteLeftBottom = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxLeftBottom);
    spriteRightBottom = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxRightBottom);
    spriteMiddle = winStyle.GetNewSprite(name + StyleSheet::LooksName::BoxMiddle);

    edges.left = (spriteLeftEdge) ? spriteLeftEdge->Size()[0] : 0;
    edges.right = (spriteRightEdge) ? spriteRightEdge->Size()[0] : 0;
    edges.top = (spriteTopEdge) ? spriteTopEdge->Size()[1] : 0;
    edges.bottom = (spriteBottomEdge) ? spriteBottomEdge->Size()[1] : 0;
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
    if (spriteLeftEdge != NULL)
        delete spriteLeftEdge;
    if (spriteTopEdge != NULL)
        delete spriteTopEdge;
    if (spriteRightEdge != NULL)
        delete spriteRightEdge;
    if (spriteBottomEdge != NULL)
        delete spriteBottomEdge;
    if (spriteLeftTop != NULL)
        delete spriteLeftTop;
    if (spriteRightTop != NULL)
        delete spriteRightTop;
    if (spriteLeftBottom != NULL)
        delete spriteLeftBottom;
    if (spriteRightBottom != NULL)
        delete spriteRightBottom;
    if (spriteMiddle != NULL)
        delete spriteMiddle;
}

void    BoxLook::Copy(const BoxLook &l)
{
    spriteLeftEdge = l.spriteLeftEdge;
    spriteTopEdge = l.spriteTopEdge;
    spriteRightEdge = l.spriteRightEdge;
    spriteBottomEdge = l.spriteBottomEdge;
    spriteLeftTop = l.spriteLeftTop;
    spriteRightTop = l.spriteRightTop;
    spriteLeftBottom = l.spriteLeftBottom;
    spriteRightBottom = l.spriteRightBottom;
    spriteMiddle = l.spriteMiddle;

    if (spriteLeftEdge != NULL)
        spriteLeftEdge = static_cast<Sprite*>(spriteLeftEdge->Clone());
    if (spriteTopEdge != NULL)
        spriteTopEdge = static_cast<Sprite*>(spriteTopEdge->Clone());
    if (spriteRightEdge != NULL)
        spriteRightEdge = static_cast<Sprite*>(spriteRightEdge->Clone());
    if (spriteBottomEdge != NULL)
        spriteBottomEdge = static_cast<Sprite*>(spriteBottomEdge->Clone());
    if (spriteLeftTop != NULL)
        spriteLeftTop = static_cast<Sprite*>(spriteLeftTop->Clone());
    if (spriteRightTop != NULL)
        spriteRightTop = static_cast<Sprite*>(spriteRightTop->Clone());
    if (spriteLeftBottom != NULL)
        spriteLeftBottom = static_cast<Sprite*>(spriteLeftBottom->Clone());
    if (spriteRightBottom != NULL)
        spriteRightBottom = static_cast<Sprite*>(spriteRightBottom->Clone());
    if (spriteMiddle != NULL)
        spriteMiddle = static_cast<Sprite*>(spriteMiddle->Clone());
}

void BoxLook::Update(const Vector2i &size)
{
    int lxLeftBottom = 0, lyLeftBottom = 0, lxLeftTop = 0, lyLeftTop = 0, lxRightBottom = 0, lyRightBottom = 0, lxRightTop = 0, lyRightTop = 0;

    // get the corner size
    if (spriteLeftBottom != NULL)
    {
        lxLeftBottom = spriteLeftBottom->TextureBox().Length(0);
        lyLeftBottom = spriteLeftBottom->TextureBox().Length(1);
    }
    if (spriteLeftTop != NULL)
    {
        lxLeftTop = spriteLeftTop->TextureBox().Length(0);
        lyLeftTop = spriteLeftTop->TextureBox().Length(1);
    }
    if (spriteRightBottom != NULL)
    {
        lxRightBottom = spriteRightBottom->TextureBox().Length(0);
        lyRightBottom = spriteRightBottom->TextureBox().Length(1);
    }
    if (spriteRightTop != NULL)
    {
        lxRightTop = spriteRightTop->TextureBox().Length(0);
        lyRightTop = spriteRightTop->TextureBox().Length(1);
    }

    // Set the size and position of the sprites
    if (spriteMiddle != NULL)
    {
        spriteMiddle->Size(Vector2f(size[0] - lxLeftBottom - lxLeftTop, size[1] - lyLeftBottom - lyLeftTop));
        spriteMiddle->Matrix.Translation(lxLeftBottom, lyLeftBottom, 0);

        if (spriteMiddle->GetColor() != color)
            spriteMiddle->SetColor(color);
    }

    if (spriteLeftTop != NULL)
    {
        spriteLeftTop->Size(Vector2f(spriteLeftTop->TextureBox().Length(0), lyLeftTop));
        spriteLeftTop->Matrix.Translation(0, size[1] - lyLeftTop, 0);

        if (spriteLeftTop->GetColor() != color)
            spriteLeftTop->SetColor(color);
    }

    if (spriteRightTop != NULL)
    {
        spriteRightTop->Size(Vector2f(lxRightTop, lyRightTop));
        spriteRightTop->Matrix.Translation(size[0] - lxRightTop, size[1] - lyRightTop, 0);

        if (spriteRightTop->GetColor() != color)
            spriteRightTop->SetColor(color);
    }

    if (spriteLeftBottom != NULL)
    {
        spriteLeftBottom->Size(Vector2f(spriteLeftBottom->TextureBox().Length(0), spriteLeftBottom->TextureBox().Length(1)));

        if (spriteLeftBottom->GetColor() != color)
            spriteLeftBottom->SetColor(color);
    }

    if (spriteRightBottom)
    {
        spriteRightBottom->Size(Vector2f(lxRightBottom, lyRightBottom));
        spriteRightBottom->Matrix.Translation(size[0] - lxRightBottom, 0, 0);

        if (spriteRightBottom->GetColor() != color)
            spriteRightBottom->SetColor(color);
    }

    if (spriteLeftEdge != NULL)
    {
        spriteLeftEdge->Size(Vector2f(spriteLeftEdge->TextureBox().Length(0), size[1] - lyLeftBottom - lyLeftTop));
        spriteLeftEdge->Matrix.Translation(0, lyLeftBottom, 0);

        if (spriteLeftEdge->GetColor() != color)
            spriteLeftEdge->SetColor(color);
    }

    if (spriteTopEdge != NULL)
    {
        float s = spriteTopEdge->TextureBox().Length(1);
        spriteTopEdge->Size(Vector2f(size[0] - lxLeftTop - lxRightTop, s));
        spriteTopEdge->Matrix.Translation(lxLeftTop, size[1] - s, 0);

        if (spriteTopEdge->GetColor() != color)
            spriteTopEdge->SetColor(color);
    }

    if (spriteRightEdge != NULL)
    {
        float s = spriteRightEdge->TextureBox().Length(0);
        spriteRightEdge->Size(Vector2f(s, size[1] - lyRightBottom - lyRightTop));
        spriteRightEdge->Matrix.Translation(size[0] - s, lyRightBottom, 0);

        if (spriteRightEdge->GetColor() != color)
            spriteRightEdge->SetColor(color);
    }

    if (spriteBottomEdge != NULL)
    {
        spriteBottomEdge->Size(Vector2f(size[0] - lxLeftBottom - lxRightBottom, spriteBottomEdge->TextureBox().Length(1)));
        spriteBottomEdge->Matrix.Translation(lxLeftBottom, 0, 0);

        if (spriteBottomEdge->GetColor() != color)
            spriteBottomEdge->SetColor(color);
    }
}

void BoxLook::Draw(Graphic::SceneGraph *scene)
{
    // render middle sprite
    if (spriteMiddle != NULL)
        spriteMiddle->RenderNode(scene);

    // render corner sprites
    if (spriteLeftTop != NULL)
        spriteLeftTop->RenderNode(scene);
    if (spriteRightTop != NULL)
        spriteRightTop->RenderNode(scene);
    if (spriteLeftBottom != NULL)
        spriteLeftBottom->RenderNode(scene);
    if (spriteRightBottom != NULL)
        spriteRightBottom->RenderNode(scene);

    // render edge sprites
    if (spriteLeftEdge != NULL)
        spriteLeftEdge->RenderNode(scene);
    if (spriteTopEdge != NULL)
        spriteTopEdge->RenderNode(scene);
    if (spriteRightEdge != NULL)
        spriteRightEdge->RenderNode(scene);
    if (spriteBottomEdge != NULL)
        spriteBottomEdge->RenderNode(scene);
}
