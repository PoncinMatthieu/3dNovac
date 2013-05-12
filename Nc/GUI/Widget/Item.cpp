
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

    File Created At:        22/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Item.h"

using namespace Nc;
using namespace Nc::GUI;
using namespace Nc::Graphic;

Item::Item(const Utils::Unicode::UTF32 &label, float fontSize, const Color &fontColor, const std::string &fontName, Graphic::PlainTextFormater::Style fontStyle)
    : Label(label, fontSize, fontColor, Left | Top, Vector2i(0,0), fontName, fontStyle), _selected(false)
{
    PaddingV(5);
    _renderRelativePos = false;
}

Item::~Item()
{
}

void    Item::Selected(bool st)
{
    if (_selected != st)
    {
        _selected = st;
        _stateChanged = true;
    }
}

