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

#ifndef NC_GRAPHICS_DATA_SPRITESHEET_H_
#define NC_GRAPHICS_DATA_SPRITESHEET_H_

#include "../GL/Texture.h"

namespace Nc
{
    namespace Graphic
    {
        /// Allow to read a sprite sheet description in Xml format
        /**
            The SpriteSheet class loads the associated texture, load boxes and associated sprite names of the sprite sheet.

            <div class="title"> Here is an example of Xml sprite sheet: </div>
                \p Examples/SpriteSheet.desc
                \include SpriteSheet.desc

                The parameter 'path' allow to setup the path of the image and every inline object 'Sprite' define a sprite with it's name and box.
                The Sprite box can then be retreive with the method 'SpriteBox'.
        */
        class LGRAPHICS SpriteSheet
        {
            public:
                typedef std::map<std::string, Box2i>    SpriteMap;

            public:
                SpriteSheet(const Utils::FileName &path);
                ~SpriteSheet();

                /** \return the sprite map. */
                inline const SpriteMap  &Sprites() const                    {return _spriteMap;}

                /** \return the texture used for every sprites. */
                inline const GL::Texture &Texture() const                   {return _texture;}

                /** \return the box associated to the given sprite name. */
                inline const Box2i      &SpriteBox(const std::string &s)    {return _spriteMap[s];}

                /** \return true if the given sprite name exist. */
                bool                    SpriteExist(const std::string &s) const;

            private:
                GL::Texture     _texture;           ///< texture of the sprite sheet.
                SpriteMap       _spriteMap;         ///< box coordinated with associated names in the sprite sheet.
        };
    }
}

#endif
