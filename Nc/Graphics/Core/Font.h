
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

    File Created At:        02/02/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Class to Load and manage Fonts files ".ttf"
    Store the font in a bitmap texture with a map of Glyphs
    connected to their values in UTF-32 unicode

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_FONT_H_
#define NC_GRAPHICS_CORE_FONT_H_

#include <map>
#include "../Define.h"
#include "GL/Texture.h"

namespace Nc
{
    namespace Graphic
    {
        /** Define a Character in a bitmap */
        struct Glyph
        {
            UInt16          Add;        ///< offset to the next character
            Vector2i        Size;       ///< size of the character
            Vector2i        Pos;        ///< relative position for caracters like `p` who begin lower than 'a'
            Box2f           Coord;      ///< coord of the character in the texture
        };

        /** Manage a font in a bitmap texture */
        class LGRAPHICS Font
        {
            private:
                typedef std::map<UInt32, Glyph>     MapGlyph;

            public:
                Font();
                ~Font();

                inline void    Enable()                             {_bitmap.Enable();}

                void    LoadFromFile(const Utils::FileName &file, unsigned int baseSize, const Utils::Unicode::UTF32 &charset = _defaultCharset);

                unsigned int        BaseSize() const                {return _baseSize;}
                void                BaseSize(unsigned int size)     {_baseSize = size;}
                const GL::Texture   &Bitmap()                       {return _bitmap;}

                const Glyph         *GetGlyph(UInt32 c) const               {MapGlyph::const_iterator it = _glyphs.find(c); return ((it != _glyphs.end()) ? &it->second : NULL);}

            private:
                unsigned int        _baseSize;              ///< the base size of the characters
                MapGlyph            _glyphs;                ///< the glyphs of the font with the their conected UTF-32 value
                GL::Texture         _bitmap;                ///< the GL texture of the font

                static UInt32   _defaultCharset[];                  ///< a default charset to load a font

                friend class FontLoaderFreeType;
        };
    }
}

#endif
