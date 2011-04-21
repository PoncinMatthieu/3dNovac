
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

#include "Button.h"
#include <Nc/Core/Engine/Manager.h>
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;
using namespace Nc::System;

Button::Button(const std::string &text, const Vector2f &pos, const Vector2f &size, Corner x, Corner y, Widget *parent, const std::string &ttf, const Utils::FileName& texture)
    : Widget(pos, size, x, y, NULL), _material(&Material<Graphic::BasicVertexType::Textured2d>::Instance()), _colorDisable(0.2, 0.2, 0.2)
{
    _textWidth = 15;
    _textHeight = 15;
    _margin.Data[0] = 4;
    _buttonPressed = false;

    _font = new Graphic::String(text, size.Data[1], Color(1, 1, 1), ttf);
    _parent = parent;
    if (_parent != NULL)
        _parent->AddChild(this);
    _drawable.texture.LoadFromFile(texture);

    _drawable.GetVBO().Init(4, GL_STREAM_DRAW);
    _material->Configure(_drawable);
    _drawable.SetPrimitiveType(GL_TRIANGLE_STRIP);
}

Button::~Button()
{
    delete _font;
}

Button::Button(const Button &w)
{
    Copy(w);
}

Button &Button::operator = (const Button &w)
{
    Copy(w);
    return *this;
}

void    Button::Copy(const Button &w)
{
    Widget::Copy(w);
    _font = new Graphic::String(*w._font);
    _textWidth = w._textWidth;
    _textHeight = w._textHeight;
    _drawable = w._drawable;
    _buttonPressed = w._buttonPressed;
}

void Button::Update()
{
    Widget::Update();
    // modifie la couleur en fonction de la selection du bouton
    Color c(1, 1, 1);
    if (!Enable())
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


    Array<BasicVertexType::Textured2d, 4>   vertices;
    vertices[0].Fill(0, 0, 0, 0, c);
    vertices[1].Fill(_size[0], 0, 1, 0, c);
    vertices[2].Fill(0, _size[1], 0, 1, c);
    vertices[3].Fill(_size[0], _size[1], 1, 1, c);
    _drawable.GetVBO().UpdateData(vertices.Data);

    // centre la font, puis l'affiche
    const Vector2f &fontSize = _font->Size();
    _font->Matrix.Translation((_size.Data[0] / 2.f) - (fontSize.Data[0] / 2.f), (_size.Data[1] / 2.f) - (fontSize.Data[1] / 2.f), 0);
}

void Button::Draw(Graphic::ISceneGraph *scene)
{
    Widget::Draw(scene);
    _material->Render(scene, _drawable);
    _font->Render(scene);
}

void Button::MouseButtonEvent(const System::Event &event)
{
    bool inRect = false;

    if (event.MouseButton.Button == System::Mouse::Left)
    {
        // test si la souris est sur le bouton
        Vector2f mousePos = WindowInput::MousePositionInGLCoord();
        Vector2f pos = GetReelPosRecursif();
        Vector2f size = GetReelSize();
        if (Math::InRect(pos, size, mousePos))
            inRect = true;

        if (inRect && !_buttonPressed && event.Type == System::Event::MouseButtonPressed)
        {
            _buttonPressed = true;
            _stateChange = true;
        }
        else if (_buttonPressed && event.Type == System::Event::MouseButtonReleased)
        {
            if (inRect)
                execHanle();
            _buttonPressed = false;
            _stateChange = true;
        }
    }
}

void Button::execHanle()
{
    if (Enable())
    {
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
        SendEvent(datas);
    }
}
