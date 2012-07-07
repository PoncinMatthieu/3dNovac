
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        25/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "WCursor.h"
#include "WWindow.h"

using namespace Nc;

Graphic::Cursor::Cursor(WWindow *w)
    : ICursor(w), _win(w), _cursor(NULL)
{
	_cursor = LoadCursor(NULL, IDC_ARROW);
}

Graphic::Cursor::~Cursor()
{
	if (_attachedWindow->CurrentCursor() == this)
		SetCurrentCursor(NULL);
	if (_win->_currentCursor == this)
        _win->_currentCursor = NULL;
}

void Graphic::Cursor::Enable()
{
	SetCursor(_cursor);
	SetCurrentCursor(this);
}

void Graphic::Cursor::Disable()
{
	SetCursor(NULL);
	SetCurrentCursor(this);
}

void Graphic::Cursor::LoadFromData(const unsigned char *data, const unsigned char *mask, const Vector2ui &size, const Vector2ui &posCenter)
{
	LOG << "WCursor::LoadFromData Not implemented, please implemente me" << std::endl;
}

void Graphic::Cursor::LoadFromXpm(const char *xpm[])
{
	LOG << "WCursor::LoadFromXmp Not implemented, please implemente me" << std::endl;
}
