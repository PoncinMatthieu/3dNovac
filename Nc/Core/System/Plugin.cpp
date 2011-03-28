
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The The 3dNovac Team

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

    File Created At:        27/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <dlfcn.h>
#include "Plugin.h"
#include "../Utils/Exception.h"

using namespace Nc::System;

Plugin::Plugin()
{
    _handle = NULL;
}

Plugin::~Plugin()
{
    Close();
}

void    Plugin::Open(const Utils::FileName &file)
{
    Close();
    _handle = dlopen(file.Fullname().c_str(), RTLD_LAZY);
    if (!_handle)
        throw Utils::Exception("Plugin", dlerror());
    dlerror();    // Clear any existing error
}

void    Plugin::Close()
{
    if (_handle)
    {
        dlclose(_handle);
        _handle = NULL;
    }
}

void    *Plugin::LoadSymbol(const std::string &sym)
{
    if (_handle == NULL)
        throw Utils::Exception("plugin", "Can't load the symbol `" + sym + "`, the library is not loaded.");

    char *error;
    void *s = dlsym(_handle, sym.c_str());

    if ((error = dlerror()) != NULL)
        throw Utils::Exception("Plugin", error);
    return s;
}
