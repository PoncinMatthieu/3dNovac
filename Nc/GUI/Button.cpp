
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
#include "WindowStyle.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;
using namespace Nc::System;

Button::Button(const std::string &text, Corner x, Corner y, const Vector2i &pos, const Vector2i &size, const std::string &ttf, const std::string &lookName)
    : Widget(x, y, pos, size)
{
    Initialize(text, size, ttf, lookName);
}

Button::Button(const std::string &text, Corner x, Corner y, const Vector2i &pos, int fontSize, int marginX, int marginY, const std::string &ttf, const std::string &lookName)
    : Widget(x, y, pos)
{
    Initialize(text, Vector2i(0, fontSize), ttf, lookName);
    _size[0] = (marginX * 2) + _font->Size()[0];
    _size[1] = (marginY * 2) + _font->Size()[1];
}

Button::~Button()
{
    delete _sprite;
    delete _font;
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
    _colorDisable = Color(0.2f, 0.2f, 0.2f);
    //_margin.Data[0] = 4; <-- why?
    _buttonPressed = false;
    _font = new Graphic::String(text, size.Data[1], Color(1, 1, 1), ttf);

    // if the sprite is not set, we take the default one in the window style
    _sprite = WindowStyle::Instance().GetNewSprite(lookName + WindowStyle::SpriteName::Button);
    if (_sprite == NULL)
        throw Utils::Exception("Button", "Cannot get the sprite '" + lookName + WindowStyle::SpriteName::Button + "' from the WindowStyle");
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
    Widget::Update();
    // modifie la couleur en fonction de la selection du bouton
    Color c(1, 1, 1);
    if (InhibitedRecursif())
    {
        c = _colorDisable;
        _font->SetColor(_colorDisable);
    }
    else
    {
        _font->SetColor(c);
        if (_buttonPressed)
            c = Color(1, 0, 0);
    }

    // update the color
    if (c != _sprite->GetColor())
        _sprite->SetColor(c);

    // update the sprite size
    if (_sprite->Size() != _size)
        _sprite->Size(_size);

    // center the font
    const Vector2f &fontSize = _font->Size();
    _font->Matrix.Translation((_size.Data[0] / 2.f) - (fontSize.Data[0] / 2.f), (_size.Data[1] / 2.f) - (fontSize.Data[1] / 2.f), 0);
}

void Button::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    _sprite->RenderNode(scene);
    _font->RenderNode(scene);
}

void Button::MouseButtonEvent(const System::Event &event)
{
    bool inRect = false;
    if (event.MouseButton.Button == System::Mouse::Left)
    {
        // test si la souris est sur le bouton
        Vector2i pos;
        Vector2i size;
        Vector2i mousePos = WindowInput::MousePositionInGLCoord();
        GetReelPosRecursif(pos);
        GetReelSize(size);
        if (Math::InRect(pos, size, mousePos))
            inRect = true;

        if (inRect && !_buttonPressed && event.Type == System::Event::MouseButtonPressed)
        {
            _buttonPressed = true;
            _stateChanged = true;
        }
        else if (_buttonPressed && event.Type == System::Event::MouseButtonReleased)
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
    {
        /*
        string datas;
        list<string> listData;
        GetParentChildData(listData);
        for (list<string>::iterator it = listData.begin(); it != listData.end();)
        {
            datas += *it;
            it++;
            if (it != listData.end())
                datas += ':';
        }
        */
        SendEvent(_id);
    }
}
