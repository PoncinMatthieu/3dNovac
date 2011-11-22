
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

Button::Button(const std::string &text, const Vector2f &pos, const Vector2f &size, Corner x, Corner y, const std::string &ttf, const Utils::FileName& texture)
    : Widget(ClassName(), pos, size, x, y), _colorDisable(0.2f, 0.2f, 0.2f)
{
    _textWidth = 15;
    _textHeight = 15;
    _margin.Data[0] = 4;
    _buttonPressed = false;
    _font = new Graphic::String(text, size.Data[1], Color(1, 1, 1), ttf);

    // creation du drawable
    _indexDrawable = _drawables.size();
    GL::GeometryBuffer<DefaultVertexType::Textured2d, false> *geometry = new GL::GeometryBuffer<DefaultVertexType::Textured2d, false>(GL::Enum::TriangleStrip);
    geometry->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    Drawable *dr = new Drawable(geometry);
    dr->Config->Textures.InitSize(1);
    dr->Config->Textures[0].LoadFromFile(texture);
    _drawables.push_back(dr);
    ChooseDefaultMaterial();
}

Button::~Button()
{
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

void    Button::Copy(const Button &w)
{
    _font = new Graphic::String(*w._font);
    _textWidth = w._textWidth;
    _textHeight = w._textHeight;
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


    Array<DefaultVertexType::Textured2d, 4>   vertices;
    vertices[0].Fill(0, 0, 0, 0, c);
    vertices[1].Fill(_size[0], 0, 1, 0, c);
    vertices[2].Fill(0, _size[1], 0, 1, c);
    vertices[3].Fill(_size[0], _size[1], 1, 1, c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(vertices.Data);

    // centre la font
    const Vector2f &fontSize = _font->Size();
    _font->Matrix.Translation((_size.Data[0] / 2.f) - (fontSize.Data[0] / 2.f), (_size.Data[1] / 2.f) - (fontSize.Data[1] / 2.f), 0);
}

void Button::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
    _font->RenderNode(scene);
}

void Button::MouseButtonEvent(const System::Event &event)
{
    bool inRect = false;

    if (event.MouseButton.Button == System::Mouse::Left)
    {
        // test si la souris est sur le bouton
        Vector2f pos;
        Vector2f size;
        Vector2f mousePos = WindowInput::MousePositionInGLCoord();
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
