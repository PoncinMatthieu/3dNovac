
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

    File Created At:        01/01/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GRAPHICS_CORE_FONT_LOADER_FREETYPE_H_
#define NC_GRAPHICS_CORE_FONT_LOADER_FREETYPE_H_

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include "FontLoader.h"

namespace Nc
{
    namespace Graphic
    {
        /// Load font files (.ttf) with the FreeType Librairy
        class LGRAPHICS FontLoaderFreeType : public FontLoader
        {
            public:
                FontLoaderFreeType();
                virtual ~FontLoaderFreeType();

                /** Load the font from a file */
                virtual void        LoadFromFile(const Utils::FileName &file, const Utils::Unicode::UTF32 &charset, Font &font);

            private:
                /** Generate the bitmap of the font */
                void CreateBitmapFont(FT_Face &face, const Utils::Unicode::UTF32 &charset, Image &bitmap, Font &font);

                FT_Library      _lib;   ///< the freetype librairy
        };
    }
}

#endif
