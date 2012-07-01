
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

    File Created At:        30/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "TextEdit.h"

using namespace Nc;
using namespace Nc::GUI;

TextEdit::TextEdit(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf)
    : ScrollArea(alignment, size)
{
    _textArea = new TextArea(text, Left | Top, size, ttf);
    AddComposedWidget(_textArea);
    SetView(_textArea);
}

TextEdit::~TextEdit()
{
}

void TextEdit::RenderChildsEnd(Graphic::SceneGraph *scene)
{
    _textArea->RenderNode(scene);
    ScrollArea::RenderChildsEnd(scene);
}

void    TextEdit::Draw(Graphic::SceneGraph *scene)
{
    ScrollArea::Draw(scene);
}
