
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Config.h"

using namespace Nc;
using namespace Nc::System;

Config::~Config()
{
    if (_file.Content() != NULL)
    {
        Save();
        delete _file.Content();
    }
}

void Config::Load(const Utils::FileName &f)
{
    if (_file.Content() != NULL)
        delete _file.Content();
    _file.Read(f);
    _content = _file.Content()->Block("Config");
}

void Config::Save()
{
    if (_file.Content() != NULL)
        _file.Save();
}
