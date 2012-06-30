
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

#include "WindowBox.h"
#include "Looks.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

WindowBox::WindowBox(const std::string &title, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const std::string &looksName)
    : Widget(alignment, size)
{
    Init(title, 18, ttf, CenterH, looksName);
}

void WindowBox::ToString(std::ostream &os) const
{
    Widget::ToString(os);
    os << " Title: " << _title->Text();
}

void  WindowBox::Init(const std::string &title, float charSize, const std::string &ttf, const AlignmentMask &mask, const std::string &looksName)
{
    UseLook(new BoxLook(looksName + WindowStyle::SpriteName::WindowBox));

    _title = NULL;
    ChangeTitle(title, charSize, ttf, mask);

    //_dragable = true;
    _stateChanged = true;
}

WindowBox::~WindowBox()
{
    delete _title;
}

WindowBox::WindowBox(const WindowBox &w)
    : Widget(w)
{
    Copy(w);
}

WindowBox &WindowBox::operator = (const WindowBox &w)
{
    if (_title != NULL)
        delete _title;

    Widget::operator = (w);
    Copy(w);
    return *this;
}

void    WindowBox::Copy(const WindowBox &w)
{
    //_dragable = w._dragable;
    _title = new Graphic::String(*w._title);
    _titleAlignment = w._titleAlignment;
    _stateChanged = true;
}

void WindowBox::ChangeTitle(const std::string &title, float charSize, const std::string &ttf, const AlignmentMask &mask)
{
    if (_title != NULL)
        delete _title;
    _title = new Graphic::String(title.c_str(), charSize, Color(0, 0, 0), ttf);
    _titleAlignment = mask;
    _stateChanged = true;
}

void WindowBox::Update()
{
    Widget::Update();

    if (_titleAlignment.Enabled(Left))
        _title->Matrix.Translation(PaddingLeft(), _size.Data[1] - static_cast<BoxLook*>(_widgetLook)->spriteTopEdge->Size()[1] + (_title->Size()[1] / 2), 0);
    else if (_titleAlignment.Enabled(CenterH))
        _title->Matrix.Translation((_size.Data[0] / 2) - (_title->Size()[0] / 2), _size.Data[1] - static_cast<BoxLook*>(_widgetLook)->spriteTopEdge->Size()[1] + (_title->Size()[1] / 2), 0);
    else
        _title->Matrix.Translation(_size.Data[0] - PaddingRight() - _title->Size()[0], _size.Data[1] - static_cast<BoxLook*>(_widgetLook)->spriteTopEdge->Size()[1] + (_title->Size()[1] / 2), 0);
}

void WindowBox::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);

    _title->RenderNode(scene);
}

