
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
#include <string.h>
#include "../System/Config.h"
#include "FileName.h"
#include "Logger.h"

using namespace std;
using namespace Nc;
using namespace Nc::Utils;

FileName::FileName(const string &path)
{
    SetFullname(path.c_str());
}

FileName::FileName(const char *path)
{
    SetFullname(path);
}

FileName &FileName::operator = (const std::string &path)
{
    SetFullname(path.c_str());
    return *this;
}

FileName &FileName::operator = (const char *path)
{
    SetFullname(path);
    return *this;
}

void   FileName::SetFullname(const char *name)
{
    if (strncmp(name, "Nc:", 3) != 0)
        string::operator=(name);
    else
    {
        string subn(name + 3);
        size_t pos = subn.find_first_of(':');
        if (pos == string::npos)
            string::operator=(name);
        else
        {
            string str = CONFIG->Block("RessourcesPath")->Line(subn.substr(0, pos))->Param("path");
            str += subn.substr(pos+1, string::npos);
            string::operator=(str);
        }
    }
}

string FileName::Path() const
{
    string::size_type Pos = find_last_of("\\/");

    if (Pos != string::npos)
        return substr(0, Pos + 1);
    else
        return *this;
}

string FileName::Filename() const
{
    string::size_type Pos = find_last_of("\\/");

    if (Pos != string::npos)
        return substr(Pos + 1, string::npos);
    else
        return *this;
}

void FileName::SetFilename(const string &s)
{
    string::size_type Pos = find_last_of("\\/");

    if (Pos != string::npos)
        *this = replace(Pos + 1, string::npos, s);
    else
        *this = s;
}

string FileName::ShortFilename() const
{
    return Filename().substr(0, Filename().find_last_of("."));
}

void FileName::SetShortFilename(const string &s)
{
    string::size_type Pos = find_last_of("\\/");
    string::size_type LastPos = find_last_of(".");
    if (Pos != string::npos)
        *this = replace(Pos + 1, LastPos - Pos - 1, s);
    else
        *this = s;
}

string FileName::Extension() const
{
    string::size_type Pos = find_last_of(".");
    if (Pos != string::npos)
        return substr(Pos + 1, string::npos);
    else
        return "";
}

bool FileName::IsReadable() const
{
    ifstream fichier(c_str());
    return !fichier.fail();
}
