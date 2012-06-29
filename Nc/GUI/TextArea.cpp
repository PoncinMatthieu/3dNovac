
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

#include "TextArea.h"

using namespace Nc;
using namespace Nc::GUI;

TextArea::TextArea(Corner x, Corner y, const Vector2i &pos, const Vector2i &size, const std::string &ttf)
{
    _editable = false;


    _text = "plopiplop";

    _string = new Graphic::String(_text, 16, Color(1, 1, 1), ttf);
}

void    TextArea::Draw(Graphic::SceneGraph *scene)
{
    ScrollArea::Draw(scene);
    _string->RenderNode(scene);
}
