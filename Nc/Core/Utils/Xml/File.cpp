
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

#include "File.h"
#include "../Exception.h"

using namespace std;
using namespace Nc::Utils;
using namespace Nc::Utils::Xml;

File::File(const FileName &filename)
{
    _filename = filename;
}

File::File(const FileName &filename, Object *content) : _content(content)
{
    _filename = filename;
}

Object  *File::Read(const FileName &f)
{
    _filename = f;
    return Read();
}

Object *File::Read()
{
    ifstream    file;

    if (_filename.IsReadable())
        file.open(_filename.c_str());
    else
        throw Exception("Xml::File", "Can't open the file " + _filename);
    _content = Parser::Parse(file);
    file.close();
    return _content;
}

void File::Save() const
{
    ofstream file(_filename.c_str());
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\" ?>" << endl;
    for (ListObject::iterator it = _content->ListChild().begin(); it != _content->ListChild().end(); ++it)
        file << **it;
    file.close();
}

void File::Save(Object *content)
{
    if (_content != content)
        _content = content;
    Save();
}
