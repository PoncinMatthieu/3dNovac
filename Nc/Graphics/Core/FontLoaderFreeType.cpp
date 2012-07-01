
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <map>
#include "FontLoaderFreeType.h"
#include "GL/Texture.h"

using namespace std;
using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::Utils;

struct GliphCmp
{
    bool operator ()(FT_BitmapGlyph Glyph1, FT_BitmapGlyph Glyph2) const
    {
        return Glyph2->bitmap.rows < Glyph1->bitmap.rows;
    }
};

FontLoaderFreeType::FontLoaderFreeType()
{
    if (FT_Init_FreeType(&_lib))
        throw Utils::Exception("FontLoaderFreeType", "Can't initialize the libFreeType.");
}

FontLoaderFreeType::~FontLoaderFreeType()
{
    FT_Done_FreeType(_lib);
}

void FontLoaderFreeType::LoadFromFile(const Utils::FileName &file, const Utils::Unicode::UTF32 &charset, Font &font)
{
	if (!file.IsReadable())
		throw Utils::Exception("FontLoaderFreeType", "The font file `" + file + "` is not readable");

	FT_Face face;
    FT_Error error = FT_New_Face(_lib, file.c_str(), 0, &face);
    if (error == FT_Err_Unknown_File_Format)
        throw Utils::Exception("FontLoaderFreeType", "Can't read the font, the format of the file `" + file + "` is not supported");
    else if (error)
        throw Utils::Exception("FontLoaderFreeType", "Can't read the font, the file `" + file + "` is probably broken");

    Image bitmap;
    CreateBitmapFont(face, charset, bitmap, font);
    FT_Done_Face(face);

    if (bitmap.Size().Data[0] > 0 && bitmap.Size().Data[1] > 0)
        font._bitmap.LoadFromImage(bitmap, GL::Enum::Texture::Linear, GL::Enum::Texture::Linear, false, file);
}

void FontLoaderFreeType::CreateBitmapFont(FT_Face &face, const Utils::Unicode::UTF32 &charset, Image &bitmap, Font &font)
{
    // Let's find how many characters to put in each row to make them fit into a squared texture
    unsigned int maxSize = GL::Texture::MaxSize();
    int nbChars = sqrt(static_cast<double>(charset.size())) * 0.75;

    // Clamp the character size to make sure we won't create a texture too big
    if (nbChars * font._baseSize >= maxSize)
        font._baseSize = maxSize / nbChars;

    // Initialize the dimensions and create the image buffer
    unsigned int left = 0, top = 0;
    // if non power of two textures is not supported, change the size
    bitmap.InitSize(Vector2ui((EXT.NonPowerOf2Supported()) ? font._baseSize * nbChars : Math::NearestPowerOf2(font._baseSize * nbChars), font._baseSize * nbChars));
    std::vector<unsigned int> tops(bitmap.Size().Data[0], 0);

    // Setup the font size
    if (FT_Set_Pixel_Sizes(face, font._baseSize, font._baseSize))
    {
        FT_Done_Face(face);
        throw Utils::Exception("FontLoaderFreeType", "Failed to set the pixel size.");
    }

    // Select the unicode character map
    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
    {
        FT_Done_Face(face);
        throw Utils::Exception("FontLoaderFreeType", "Failed to select the charmap");
    }

    // Render all glyphs and sort them by size to optimize texture space
    multimap<FT_BitmapGlyph, UInt32, GliphCmp> glyphs;
    for (size_t i = 0; i < charset.size(); ++i)
    {
        // Load the glyph corresponding to the current character
        if (FT_Load_Char(face, charset[i], FT_LOAD_TARGET_NORMAL))
        {
            FT_Done_Face(face);
            throw Utils::Exception("FontLoaderFreeType", "Failed to load the char number " + Convert::ToString(i) + " from charset");
        }
        // Convert the glyph to a bitmap
        FT_Glyph glyph;
        if (FT_Get_Glyph(face->glyph, &glyph))
        {
            FT_Done_Face(face);
            throw Utils::Exception("FontLoaderFreeType", "Failed to get the glyphnumber " + Convert::ToString(i) + " from charset");
        }
        FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, 0, 1);
        FT_BitmapGlyph bitmapGlyph = (FT_BitmapGlyph)glyph;

        // Add it to the sorted table of glyphs
        glyphs.insert(std::make_pair(bitmapGlyph, charset[i]));
    }

    // Copy the rendered glyphs into the texture
    unsigned int                maxHeight = 0;
    std::map<UInt32, Box2i>     coords;
    for (std::multimap<FT_BitmapGlyph, UInt32, GliphCmp>::const_iterator it = glyphs.begin(); it != glyphs.end(); ++it)
    {
        // Get the bitmap of the current glyph
        Glyph                   &curGlyph = font._glyphs[it->second];
        const FT_BitmapGlyph    &bitmapGlyph = it->first;
        FT_Bitmap               &bt = bitmapGlyph->bitmap;

        // Make sure we don't go over the texture width
        if (left + bt.width + 1 >= bitmap.Size().Data[0])
            left = 0;

        // Compute the top coordinate
        top = tops[left];
        for (int x = 0; x < bt.width + 1; ++x)
            top = max(top, tops[left + x]);
        top++;

        // Make sure we don't go over the texture height -- resize it if we need more space
        if (top + bt.rows + 1 >= bitmap.Size().Data[1])
            bitmap.Resize(bitmap.Size() * 2);

        // Store the character's position and size
        curGlyph.Pos.Data[0] = bitmapGlyph->left;
        curGlyph.Pos.Data[1] = bt.rows - bitmapGlyph->top;
        curGlyph.Size.Data[0] = bt.width;
        curGlyph.Size.Data[1] = -bt.rows;
        curGlyph.Add = bitmapGlyph->root.advance.x >> 16;

        // Texture size may change, so let the texture coordinates be calculated later
        coords[it->second] = Box2i(left + 1, top + 1, left + bt.width + 1, top + bt.rows + 1);

        // Draw the glyph into our bitmap font
        const unsigned char *pixels = static_cast<const unsigned char*>(bt.buffer);
        Pixel<> *bitmapPixel = bitmap.GetPixelsArray();
        for (int y = 0; y < bt.rows; ++y)
        {
            for (int x = 0; x < bt.width; ++x)
            {
                size_t index = x + left + 1 + (y + top + 1) * bitmap.Size().Data[0];
                bitmapPixel[index].R = 255;
                bitmapPixel[index].G = 255;
                bitmapPixel[index].B = 255;
                bitmapPixel[index].A = pixels[x];
            }
            pixels += bt.pitch;
        }

        // Update the rendering coordinates
        for (int x = 0; x < bt.width + 1; ++x)
            tops[left + x] = top + bt.rows;
        left += bt.width + 1;
        if (top + bt.rows > maxHeight)
            maxHeight = top + bt.rows;

        // Delete the glyph
        FT_Done_Glyph((FT_Glyph)bitmapGlyph);
    }

    // Create the font's texture
    bitmap.Resize(Vector2ui(bitmap.Size().Data[0], maxHeight + 1));

    // Now that the texture is created, we can precompute texture coordinates
    for (size_t i = 0; i < charset.size(); ++i)
    {
        UInt32  curChar = charset[i];
        Box2i   &box = coords[curChar];
        font._glyphs[curChar].Coord.Min(0, (float)box.Min(0) / (float)bitmap.Size().Data[0]);
        font._glyphs[curChar].Coord.Min(1, (float)box.Min(1) / (float)bitmap.Size().Data[1]);
        font._glyphs[curChar].Coord.Max(0, (float)box.Max(0) / (float)bitmap.Size().Data[0]);
        font._glyphs[curChar].Coord.Max(1, (float)box.Max(1) / (float)bitmap.Size().Data[1]);
    }
}
