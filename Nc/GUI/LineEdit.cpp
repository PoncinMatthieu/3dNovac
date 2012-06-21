
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

#include "LineEdit.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

LineEdit::LineEdit(const std::string &label, Corner x, Corner y, const Vector2i &pos, const Vector2i &size, const std::string &ttf, const std::string &looksName)
    : WidgetLabeled(label, size[1], x, y, pos, size, ttf)
{
    _margin[0] = 4;
    _centerLabelY = true;
    UpdateLabel();
    _font = new Graphic::String("", size[1], Color(1, 1, 1), ttf);

    // creation des drawable
    _indexDrawable = _drawables.size();
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry1 = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::LineLoop);
    geometry1->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    _drawables.push_back(new Drawable(geometry1));
    ChooseDefaultMaterial();
}

LineEdit::~LineEdit()
{
    delete _font;
}

LineEdit::LineEdit(const LineEdit &w)
    : WidgetLabeled(w)
{
    Copy(w);
}

LineEdit &LineEdit::operator = (const LineEdit &w)
{
    WidgetLabeled::operator = (w);
    Copy(w);
    return *this;
}

void    LineEdit::Copy(const LineEdit &w)
{
    _font = new Graphic::String(*w._font);
}

void    LineEdit::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Text: " << _font->Text();
}

void    LineEdit::Update()
{
    WidgetLabeled::Update();

    Color c(0,0,1);
    if (!_focus)
        c = Color(0,1,0);

    Array<DefaultVertexType::Colored2d, 4>   vertices;
    vertices[0].Fill(_label->Size().Data[0], 0, c);
    vertices[1].Fill(_label->Size().Data[0], _size.Data[1], c);
    vertices[2].Fill(_label->Size().Data[0] + _size.Data[0], _size.Data[1], c);
    vertices[3].Fill(_label->Size().Data[0] + _size.Data[0], 0, c);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[_indexDrawable]->Geometry)->VBO().UpdateData(vertices.Data);

    _font->Matrix.Translation(_label->Size().Data[0], (_size.Data[1] / 2.f) - (_font->Size().Data[1] / 2.f), 0);
}

void LineEdit::Draw(Graphic::SceneGraph *scene)
{
    WidgetLabeled::Draw(scene);
    GetMaterial()->Render(scene, *_drawables[_indexDrawable]);
    _font->RenderNode(scene);
}

void LineEdit::KeyboardEvent(const System::Event &event)
{
    if (event.type == System::Event::KeyPressed)
    {
        if (event.key.Code == System::Key::Back && !_font->Text().empty()) // suppression du dernier caractere
        {
            Utils::Unicode::UTF32 s = _font->Text();
            s.erase(s.end() - 1);
            _font->Text(s);
        }
        else
        {
            UInt32 c = (UInt32)WindowInput::ToChar(event.key.Code);
            if (c != '\0' && _font->Size().Data[0] + _font->GetCharSize(c).Data[0] < _size.Data[0]) // ajout du caractere dans la string)
            {
                Utils::Unicode::UTF32 s = _font->Text();
                s += c;
                _font->Text(s);
            }
        }
        _stateChanged = true;
    }
}

void LineEdit::GetData(std::string &data)
{
    data = _font->Text().ToStdString();
}
