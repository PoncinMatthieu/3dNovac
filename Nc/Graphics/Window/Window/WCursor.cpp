
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

System::Cursor::Cursor(WWindow *w)
    : _win(w), _cursor(NULL)
{
	_cursor = LoadCursor(NULL, IDC_ARROW);
}

System::Cursor::~Cursor()
{
    if (_win->_currentCursor == this)
        _win->_currentCursor = NULL;
}

void System::Cursor::Enable()
{
	SetCursor(_cursor);
	_win->_currentCursor = this;
}

void System::Cursor::Disable()
{
	SetCursor(NULL);
	_win->_currentCursor = this;
}

void System::Cursor::LoadFromData(const unsigned char *data, const unsigned char *mask, const Vector2ui &size, const Vector2ui &posCenter)
{
	LOG << "WCursor::LoadFromData Not implemented, please implemente me" << std::endl;
}

void System::Cursor::LoadFromXpm(const char *xpm[])
{
	LOG << "WCursor::LoadFromXmp Not implemented, please implemente me" << std::endl;
}
