
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

WidgetLabeled::WidgetLabeled(const Utils::Unicode::UTF32 &label, const Vector2i &pos, const Vector2i &size, Corner x, Corner y, const std::string &ttf, Graphic::String::Style s)
    : Widget(ClassName(), pos, size, x, y), _label(NULL)
{
    CreateLabel(label, ttf, s);
}

WidgetLabeled::WidgetLabeled(const char *className, const Utils::Unicode::UTF32 &label, const Vector2i &pos, const Vector2i &size, Corner x, Corner y, const std::string &ttf, Graphic::String::Style s)
    : Widget(className, pos, size, x, y), _label(NULL)
{
    CreateLabel(label, ttf, s);
}

WidgetLabeled::~WidgetLabeled()
{
    if (_label != NULL)
        delete _label;
}

WidgetLabeled::WidgetLabeled(const WidgetLabeled &w)
    : Widget(w)
{
    Copy(w);
}

WidgetLabeled &WidgetLabeled::operator = (const WidgetLabeled &w)
{
    if (_label != NULL)
        delete _label;

    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    WidgetLabeled::Copy(const WidgetLabeled &w)
{
    _label = new Graphic::String(*w._label);
}

void WidgetLabeled::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Label: " << _label->Text();
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
    _label = new Graphic::String(l, _size.Data[1], Color(1, 0, 0), ttf, s);
    _stateChanged = true;
}

void WidgetLabeled::Draw(Graphic::SceneGraph *scene)
{
    // draw le label
    _label->RenderNode(scene);

    // draw le widget
    Widget::Draw(scene);
}

void WidgetLabeled::GetReelSize(Vector2f &size) const
{
    Widget::GetReelSize(size);
    size[0] += _label->Size().Data[0];
    float s = _label->Size().Data[1];
    if (s > size[1])
        size[1] = s;
}
