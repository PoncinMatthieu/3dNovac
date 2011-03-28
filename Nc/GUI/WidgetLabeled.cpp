
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

#include "WidgetLabeled.h"

#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;

WidgetLabeled::WidgetLabeled(const Utils::Unicode::UTF32 &label, const Vector2i &pos, const Vector2i &size, Corner x, Corner y, Widget *parent, const std::string &ttf, Graphic::String::Style s)
    : Widget(pos, size, x, y, NULL), _label(NULL)
{
    _parent = parent;
    CreateLabel(label, ttf, s);
    if (_parent != NULL)
        _parent->AddChild(this);
}

WidgetLabeled::~WidgetLabeled()
{
    if (_label != NULL)
        delete _label;
}

WidgetLabeled::WidgetLabeled(const WidgetLabeled &w)
{
    Copy(w);
}

WidgetLabeled &WidgetLabeled::operator = (const WidgetLabeled &w)
{
    Copy(w);
    return *this;
}

void    WidgetLabeled::Copy(const WidgetLabeled &w)
{
    Widget::Copy(w);
    _label = new Graphic::String(*w._label);
}

void WidgetLabeled::Size(const Vector2f &size)
{
    Widget::Size(size);
    _label->CharSize(_size.Data[1]);
}

void WidgetLabeled::CreateLabel(const Utils::Unicode::UTF32 &l, const std::string &ttf, Graphic::String::Style s)
{
    if (_label)
        delete _label;
    _label = new Graphic::String(l, _size.Data[1], Color(255, 0, 0), ttf, s);
    _stateChange = true;
}

void WidgetLabeled::Draw(Graphic::ISceneGraph *scene)
{
    // draw le label
    _label->Render(scene);

    // draw le widget
    Widget::Draw(scene);
}

Vector2f WidgetLabeled::GetReelSize() const
{
    Vector2f v = Widget::GetReelSize();
    v[0] += _label->Size().Data[0];
    float s = _label->Size().Data[1];
    if (s > v[1])
        v[1] = s;
    return v;
}
