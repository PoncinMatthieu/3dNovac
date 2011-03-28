
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

#include "TextBox.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

TextBox::TextBox(const std::string &label, const Vector2f &pos, const Vector2f &size, Corner x, Corner y, Widget *parent, const std::string &ttf)
    : WidgetLabeled(label, pos, size, x, y, parent, ttf)
{
    _margin.Data[0] = 4;
    _font = new Graphic::String("", size.Data[1], Color(1, 1, 1), ttf);

    _geometry.GetVBO().Init(4, GL_STREAM_DRAW);
    _materialColored2d->Configure(_geometry);
    _geometry.SetPrimitiveType(GL_LINE_LOOP);
}

TextBox::~TextBox()
{
    delete _font;
}

TextBox::TextBox(const TextBox &w) : WidgetLabeled(w)
{
    Copy(w);
}

TextBox &TextBox::operator = (const TextBox &w)
{
    Copy(w);
    return *this;
}

void    TextBox::Copy(const TextBox &w)
{
    Widget::Copy(w);
    _font = new Graphic::String(*w._font);
    _text = w._text;
}

void    TextBox::Update()
{
    WidgetLabeled::Update();

    Color c(0,0,1);
    if (!_focus)
        c = Color(0,1,0);

    Array<BasicVertexType::Colored2d, 4>   vertices;
    vertices[0].Fill(_label->Size().Data[0], 0, c);
    vertices[1].Fill(_label->Size().Data[0], _size.Data[1], c);
    vertices[2].Fill(_label->Size().Data[0] + _size.Data[0], _size.Data[1], c);
    vertices[3].Fill(_label->Size().Data[0] + _size.Data[0], 0, c);
    _geometry.GetVBO().UpdateData(vertices.Data);

    _font->Matrix.Translation(_label->Size().Data[0], (_size.Data[1] / 2.f) - (_font->Size().Data[1] / 2.f), 0);
}

void TextBox::Draw(Graphic::ISceneGraph *scene)
{
    WidgetLabeled::Draw(scene);
    _materialColored2d->Render(scene, _geometry, _config);
    _font->Render(scene);
}

void TextBox::KeyboardEvent(const System::Event &event)
{
    if (event.Type == System::Event::KeyPressed)
    {
        if (event.Key.Code == System::Key::Back && !_text.empty()) // suppression du dernier caractere
        {
            _text.erase(_text.end() - 1);
            _font->Text(_text.c_str());
        }
        else
        {
            char c = WindowInput::ToChar(event.Key.Code);
            if (c != '\0' && _font->Size().Data[0] + _font->GetCharSize(c).Data[0] < _size.Data[0]) // ajout du caractere dans la string)
            {
                _text += c;
                _font->Text(_text.c_str());
            }
        }
        _stateChange = true;
    }
}

void TextBox::Text(const string &t)
{
    _text = t;
    _font->Text(_text.c_str());
}

string TextBox::GetData()
{
    return _text;
}
