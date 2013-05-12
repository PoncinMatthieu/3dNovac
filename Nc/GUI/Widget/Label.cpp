
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

#include <Nc/Graphics/Object/Sprite.h>
#include "Label.h"

#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;

Label::Label(const Utils::Unicode::UTF32 &label, float charSize, const Color &c, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, Graphic::PlainTextFormater::Style s)
    : Widget(alignment, size), _label(NULL), _icon(NULL), _labelAlignment(Left | CenterV), _highlight(false), _highlightSprite(NULL)
{
    PaddingH(5);
    Text(label, charSize, c, ttf, s);
}

Label::~Label()
{
    if (_label != NULL)
        delete _label;
    if (_icon != NULL)
        delete _icon;
    if (_highlightSprite != NULL)
        delete _highlightSprite;
}

Label::Label(const Label &w)
    : Widget(w)
{
    Copy(w);
}

Label &Label::operator = (const Label &w)
{
    if (_label != NULL)
        delete _label;
    if (_icon != NULL)
        delete _icon;
    if (_highlightSprite != NULL)
        delete _highlightSprite;

    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    Label::Copy(const Label &w)
{
    _highlight = w._highlight;
    _highlightColor = w._highlightColor;
    _highlightSprite = NULL;
    if (w._label != NULL)
        _label = new Graphic::Text(*w._label);
    if (w._icon != NULL)
        _icon = new Graphic::Sprite(*w._icon);
}

void Label::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    if (_label != NULL)
        os << " Label: " << _label->PlainText();
}

void Label::Text(const Utils::Unicode::UTF32 &l, float charSize, const Color &c, const std::string &ttf, Graphic::PlainTextFormater::Style s)
{
    if (_label != NULL)
        delete _label;
    _label = new Graphic::Text(l, charSize, c, ttf, s);
    _stateChanged = true;
}

void Label::Icon(const Graphic::GL::Texture &texture, const Box2i &textureBox)
{
    if (_icon != NULL)
        delete _icon;
    _icon = new Graphic::Sprite(textureBox.Length(), texture, textureBox);
}

void Label::Icon(const Graphic::GL::Texture &texture)
{
    if (_icon != NULL)
        delete _icon;
    _icon = new Graphic::Sprite(texture.Size(), texture, 100);
}

void Label::UpdateState()
{
    RecomputeWidgetSize();

    Widget::UpdateState();
    if (_label != NULL)
        UpdateLabel();
    if (_icon != NULL)
        UpdateIcon();
}

void Label::RecomputeWidgetSize()
{
    int minX = 0;
    int minY = 0;
    if (_label != NULL)
    {
        minX += _label->Size()[0];
        minY += _label->Size()[1];
    }
    if (_icon != NULL)
    {
        minX += _icon->Size()[0];
        if (minY < _icon->Size()[1])
            minY = _icon->Size()[1];
    }
    minX += PaddingLeft() + PaddingRight();
    minY += PaddingTop() + PaddingBottom();

    if (_size[0] < minX)
    {
        _size[0] = minX;
        Resized();
    }
    if (_size[1] < minY)
    {
        _size[1] = minY;
        Resized();
    }
}

void Label::UpdateLabel()
{
    _label->Matrix.SetIdentity();

    int xOffset = 0;
    if (_icon != NULL)
        xOffset += _icon->Size()[0] + 2;

    // manage horizontal alignment
    if (_labelAlignment.Enabled(Left))
    {
        _label->Matrix.Translation(xOffset + PaddingLeft(), 0, 0);
    }
    else if (_labelAlignment.Enabled(CenterH))
    {
        _label->Matrix.Translation((_size.data[0] / 2.f) - ((xOffset + _label->Size().data[0]) / 2.f), 0, 0);
    }
    else if (_labelAlignment.Enabled(Right))
    {
        _label->Matrix.Translation(_size.data[0] - _label->Size().data[0] - xOffset - PaddingRight(), 0, 0);
    }

    // manage vertical alignment
    if (_labelAlignment.Enabled(Bottom))
    {
        _label->Matrix.Translation(PaddingBottom(), 0, 0);
    }
    else if (_labelAlignment.Enabled(CenterV))
    {
        _label->Matrix.AddTranslation(0, (_size.data[1] / 2.f) - (_label->Size().data[1] / 2.f), 0);
    }
    else if (_labelAlignment.Enabled(Top))
    {
        _label->Matrix.AddTranslation(0, _size.data[1] - _label->Size().data[1] - PaddingTop(), 0);
    }

    if (_highlight)
    {
        if (_highlightSprite == NULL)
        {
            _highlightSprite = new Graphic::Sprite(_size, Graphic::GL::Texture(), 100);
        }
        _highlightSprite->SetColor(_highlightColor);
    }
    else if (_highlightSprite != NULL)
    {
        delete _highlightSprite;
        _highlightSprite = NULL;
    }
}

void Label::UpdateIcon()
{
    _icon->Matrix.SetIdentity();

    // manage horizontal alignment
    if (_labelAlignment.Enabled(Left))
    {
        _icon->Matrix.Translation(PaddingLeft(), 0, 0);
    }
    else if (_labelAlignment.Enabled(CenterH))
    {
        _icon->Matrix.Translation((_size.data[0] / 2.f) - (_icon->Size().data[0] / 2.f), 0, 0);
    }
    else if (_labelAlignment.Enabled(Right))
    {
        _icon->Matrix.Translation(_size.data[0] - _icon->Size().data[0] - PaddingRight(), 0, 0);
    }

    // manage vertical alignment
    if (_labelAlignment.Enabled(Bottom))
    {
        _icon->Matrix.Translation(PaddingBottom(), 0, 0);
    }
    else if (_labelAlignment.Enabled(CenterV))
    {
        _icon->Matrix.AddTranslation(0, (_size.data[1] / 2.f) - (_icon->Size().data[1] / 2.f), 0);
    }
    else if (_labelAlignment.Enabled(Top))
    {
        _icon->Matrix.AddTranslation(0, _size.data[1] - _icon->Size().data[1] - PaddingTop(), 0);
    }
}

void Label::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    if (_highlightSprite != NULL)
        _highlightSprite->RenderNode(scene);
    if (_icon != NULL)
        _icon->RenderNode(scene);
    if (_label != NULL)
        _label->RenderNode(scene);
}

void    Label::PosChild(const Widget *w, Vector2i &pos) const
{
    Widget::PosChild(w, pos);

    if (_label != NULL)
    {
        if (_labelAlignment.Enabled(Left))
        {
            if (w->Alignment().Enabled(Left))
                pos[0] += _label->Size()[0];
            else if (w->Alignment().Enabled(CenterH))
                pos[0] += _label->Size()[0] / 2;
        }
        else if (_labelAlignment.Enabled(Right))
        {
            if (w->Alignment().Enabled(Right))
                pos[0] -= _label->Size()[0];
            else if (w->Alignment().Enabled(CenterH))
                pos[0] -= _label->Size()[0] / 2;
        }
    }
}

void    Label::SizeChild(const Widget *w, Vector2i &size) const
{
    Widget::SizeChild(w, size);
    if (_label != NULL)
        size[0] -= _label->Size()[0];
}

void    Label::AddBuddy(Widget *w)
{
    w->Percent(Vector2f(100, 100));
    AddChild(w);
}
