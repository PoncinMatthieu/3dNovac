
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

    File Created At:        28/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "TextDocument.h"
#include "../Visitor/Visitors.h"

using namespace Nc;
using namespace Nc::GUI;

TextDocument::TextDocument(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf)
    : Widget(alignment, size)
{
    _editable = false;
    _string = new Graphic::String("", 16, Color(1, 1, 1), ttf);
    Text(text);

/// a sup
    Resize();
}

void    TextDocument::Text(const Utils::Unicode::UTF32 &t)
{
    _string->Text(t);
    Resized();
}

void    TextDocument::Resize()
{
    if (_size != _string->Size())
    {
        _size = _string->Size();
    }
}

void    TextDocument::Update()
{
    Widget::Update();
}

void    TextDocument::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);
    _string->RenderNode(scene);
}
