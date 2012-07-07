
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

    File Created At:        09/02/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "SpriteSheet.h"

using namespace Nc;
using namespace Nc::Utils;
using namespace Nc::Graphic;

SpriteSheet::SpriteSheet(const Utils::FileName &path)
{
    // read the file and extract the SpriteShit description
    Xml::File spriteSheet(path);
    Xml::Object *content = spriteSheet.Read();
    content = content->Block("SpriteSheet");

    // load associated texture
    std::string &s = content->Param("path");
    _texture.LoadFromFile(s, GL::Enum::Texture::Nearest, GL::Enum::Texture::Nearest, false);

    // retreive every sprites
    for (Xml::ListObject::iterator it = content->ListChild().begin(); it != content->ListChild().end(); ++it)
    {
        // get the box
        int minX, minY, maxX, maxY;
        Utils::Convert::StringTo((*it)->Param("topX"), minX);
        Utils::Convert::StringTo((*it)->Param("topY"), minY);
        Utils::Convert::StringTo((*it)->Param("bottomX"), maxX);
        Utils::Convert::StringTo((*it)->Param("bottomY"), maxY);

        // record the box
        _spriteMap[(*it)->Param("name")] = Box2i(minX, _texture.Size()[1] - maxY,
                                                 maxX, _texture.Size()[1] - minY);
    }
}

SpriteSheet::~SpriteSheet()
{
}

bool    SpriteSheet::SpriteExist(const std::string &s) const
{
    SpriteMap::const_iterator it = _spriteMap.find(s);
    if (it != _spriteMap.end())
        return true;
    return false;
}

