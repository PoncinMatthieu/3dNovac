
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


#include <fstream>
#include "FileName.h"

using namespace std;
using namespace Nc;
using namespace Nc::Utils;

FileName::FileName(const std::string& path)
{
    _path = path;
}

FileName::FileName(const char *path)
{
    _path = string(path);
}

bool FileName::operator == (const FileName& f) const
{
    if (_path == f._path)
        return true;
    return false;
}

std::string FileName::Path() const
{
    std::string::size_type Pos = _path.find_last_of("\\/");

    if (Pos != std::string::npos)
        return _path.substr(0, Pos + 1);
    else
        return _path;
}

std::string FileName::Filename() const
{
    std::string::size_type Pos = _path.find_last_of("\\/");

    if (Pos != std::string::npos)
        return _path.substr(Pos + 1, std::string::npos);
    else
        return _path;
}

void FileName::SetFilename(std::string s)
{
    std::string::size_type Pos = _path.find_last_of("\\/");

    if (Pos != std::string::npos)
        _path = _path.replace(Pos + 1, std::string::npos, s);
    else
        _path = s;
}

std::string FileName::ShortFilename() const
{
    return Filename().substr(0, Filename().find_last_of("."));
}

std::string FileName::Extension() const
{
    std::string::size_type Pos = _path.find_last_of(".");
    if (Pos != std::string::npos)
        return _path.substr(Pos + 1, std::string::npos);
    else
        return "";
}

bool FileName::IsReadable() const
{
    ifstream fichier(_path.c_str());
    return !fichier.fail();
}
