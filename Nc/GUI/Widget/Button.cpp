
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
    DeleteButton();
}

Button::Button(const Button &w)
    : Widget(w)
{
    Copy(w);
}

Button &Button::operator = (const Button &w)
{
    DeleteButton();
    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    Button::DeleteButton()
{
    delete _font;

    if (_widgetLook != NULL && (_widgetLook == _buttonLook || _widgetLook == _buttonLookPressed))
        _widgetLook = NULL;

    delete _buttonLook;
    delete _buttonLookPressed;
}

void    Button::Initialize(const std::string &text, const Vector2i &size, const std::string &ttf, const std::string &lookName)
{
    _buttonLook = new StripLook(lookName + StyleSheet::Name::Button);
    _buttonLookPressed = new StripLook(lookName + StyleSheet::Name::ButtonPressed);
    UseLook(_buttonLook);

    _colorDisable = Color(0.2f, 0.2f, 0.2f);
    _buttonPressed = false;
    _charSize = size.data[1] - (PaddingTop() + PaddingBottom());
    _font = new Graphic::Text(text, _charSize, Color(1, 1, 1), ttf);
}

void    Button::Copy(const Button &w)
{
    _buttonLook = static_cast<StripLook*>(w._buttonLook->Clone());
    _buttonLookPressed = static_cast<StripLook*>(w._buttonLookPressed->Clone());
    UseLook(_buttonLook);

    _colorDisable = w._colorDisable;
    _buttonPressed = w._buttonPressed;
    _font = new Graphic::Text(*w._font);
    _charSize = w._charSize;
}

void Button::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Text: " << _font->PlainText();
}

void Button::Update()
{
    Graphic::PlainTextFormater *formater = static_cast<Graphic::PlainTextFormater*>(_font->Formater());

    // update the color
    Color c(1, 1, 1);
    if (InhibitedRecursif())
        c = _colorDisable;
    formater->SetColor(c);

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
    formater->SetCharSize(_charSize);
    while (!st && formater->GetCharSize() > 0)
    {
        if (_font->Size().data[0] > (_size.data[0] - PaddingH()) ||
            _font->Size().data[1] > (_size.data[1] - PaddingV()))
            formater->SetCharSize(formater->GetCharSize() - 1);
        else
            st = true;
    }

    // center the font
    const Vector2f &fontSize = _font->Size();
    _font->Matrix.Translation(PaddingLeft() + ((_size.data[0] - PaddingH()) / 2.f) - (fontSize.data[0] / 2.f),
                              PaddingTop() + ((_size.data[1] - PaddingV()) / 2.f) - (fontSize.data[1] / 2.f), 0);
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
        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        AbsolutePos(pos);
        if (Math::InRect(pos, _size, mousePos))
            inRect = true;

        if (inRect && !_buttonPressed && event.type == System::Event::MouseButtonPressed)
        {
            _buttonPressed = true;
            _stateChanged = true;
        }
        else if (_buttonPressed && event.type == System::Event::MouseButtonReleased)
        {
            if (inRect)
                ExecHanle();
            _buttonPressed = false;
            _stateChanged = true;
        }
    }
}

void Button::ExecHanle()
{
    SendEvent(_id);
}
