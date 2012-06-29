
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

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Object/Sprite.h>
#include "Button.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;
using namespace Nc::System;

Button::Button(const std::string &text, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const std::string &lookName)
    : Widget(alignment, size)
{
    Initialize(text, size, ttf, lookName);
}

Button::~Button()
{
    delete _font;

    if (_widgetLook != NULL && (_widgetLook == _buttonLook || _widgetLook == _buttonLookPressed))
        _widgetLook = NULL;

    delete _buttonLook;
    delete _buttonLookPressed;
}

Button::Button(const Button &w)
    : Widget(w)
{
    Copy(w);
}

Button &Button::operator = (const Button &w)
{
    Widget::operator = (w);
    Copy(w);
    return *this;
}


void    Button::Initialize(const std::string &text, const Vector2i &size, const std::string &ttf, const std::string &lookName)
{
    _buttonLook = new StripLook(lookName + WindowStyle::SpriteName::Button);
    _buttonLookPressed = new StripLook(lookName + WindowStyle::SpriteName::ButtonPressed);
    UseLook(_buttonLook);

    _colorDisable = Color(0.2f, 0.2f, 0.2f);
    _buttonPressed = false;
    _charSize = size.Data[1] - (_margin.top + _margin.bottom) - (_buttonLook->edges.top + _buttonLook->edges.bottom);
    _font = new Graphic::String(text, _charSize, Color(1, 1, 1), ttf);
}

void    Button::Copy(const Button &w)
{
    _font = new Graphic::String(*w._font);
    _buttonPressed = w._buttonPressed;
}

void Button::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Text: " << _font->Text();
}

void Button::Update()
{
    // update the color
    Color c(1, 1, 1);
    if (InhibitedRecursif())
    {
        c = _colorDisable;
        _font->SetColor(_colorDisable);
    }

    if (_font->GetColor() != c)
        _font->SetColor(c);

    if (_widgetLook != NULL)
        _widgetLook->color = c;

    // update the widget look, if pressed
    if (_buttonPressed)
        _widgetLook = _buttonLookPressed;
    else
        _widgetLook = _buttonLook;

    // update the widget, which will update the look
    Widget::Update();

    // update the size of the title
    // we have to reduce the size of the text if it's too large
    bool st = false;
    if (_font->CharSize() != _charSize)
        _font->CharSize(_charSize);
    while (!st && _font->CharSize() > 0)
    {
        if (_font->Size().Data[0] > (_size.Data[0] - _margin.left - _margin.right - _buttonLook->edges.left - _buttonLook->edges.right) ||
            _font->Size().Data[1] > (_size.Data[1] - _margin.top - _margin.bottom - _buttonLook->edges.top - _buttonLook->edges.bottom))
            _font->CharSize(_font->CharSize() - 1);
        else
            st = true;
    }

    // center the font
    const Vector2f &fontSize = _font->Size();
    _font->Matrix.Translation((_size.Data[0] / 2.f) - (fontSize.Data[0] / 2.f), (_size.Data[1] / 2.f) - (fontSize.Data[1] / 2.f), 0);
}

void Button::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    _font->RenderNode(scene);
}

void Button::MouseButtonEvent(const System::Event &event)
{
    bool inRect = false;
    if (event.mouseButton.button == System::Mouse::Left)
    {
        // test si la souris est sur le bouton
        Vector2i pos;
        Vector2i size;
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        GetReelPosRecursif(pos);
        GetReelSize(size);
        if (Math::InRect(pos, size, mousePos))
            inRect = true;

        if (inRect && !_buttonPressed && event.type == System::Event::MouseButtonPressed)
        {
            _buttonPressed = true;
            _stateChanged = true;
        }
        else if (_buttonPressed && event.type == System::Event::MouseButtonReleased)
        {
            if (inRect)
                execHanle();
            _buttonPressed = false;
            _stateChanged = true;
        }
    }
}

void Button::execHanle()
{
    if (!InhibitedRecursif())
        SendEvent(_id);
}
