/*
	3dNovac System
	Copyright (C) 2010, Poncin Matthieu
	All rights reserved.

	This work is licensed under the Creative Commons
	Attribution-Share Alike 2.0 France License.
	To view a copy of this license, visit :
	http://creativecommons.org/licenses/by-sa/2.0/fr/
*/

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
