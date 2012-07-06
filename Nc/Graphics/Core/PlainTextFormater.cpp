
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

    File Created At:        01/07/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "PlainTextFormater.h"
#include "Drawable.h"

using namespace Nc;
using namespace Nc::Graphic::Core;

FontMap         PlainTextFormater::_mapFont;

PlainTextFormater::PlainTextFormater(float charSize, const Color &color, const std::string &ttf, const Utils::Mask<Style> &s)
    : _style(s), _charSize(charSize), _color(color), _documentSize(0), _alignment(Left)
{
    // search the font in the map
    FontMap::iterator it = _mapFont.find(ttf);
    if (it == _mapFont.end()) // si on ne la pas, on creer la font
    {
        Utils::FileName s = CONFIG->Block("RessourcesPath")->Line("Font")->Param("path") + ttf + ".ttf";
        _mapFont[ttf] = _font = new Core::Font();
        _font->LoadFromFile(s, CHAR_SIZE);
    }
    else
        _font = it->second;
}

PlainTextFormater::~PlainTextFormater()
{

}

void PlainTextFormater::DestroyFonts()
{
    while (!_mapFont.empty())
    {
        delete _mapFont.begin()->second;
        _mapFont.erase(_mapFont.begin());
    }
}

void    PlainTextFormater::SetColor(const Color &color)
{
    if (_color != color)
    {
        _color = color;
        _needUpdate = true;
    }
}

void    PlainTextFormater::SetCharSize(float size)
{
    if (_charSize != size)
    {
        _charSize = size;
        _needUpdate = true;
    }
}

Vector2f PlainTextFormater::GetCharSize(UInt32 c) const
{
    float           factor = _charSize / _font->BaseSize();
    const Glyph     *r = _font->GetGlyph(c);
    return (r != NULL) ? Vector2f(r->Size.data[0] * factor, r->Size.data[1] * factor) : Vector2f(0, _charSize);
}

void    PlainTextFormater::SetDocumentSize(float size)
{
    if (_documentSize != size)
    {
        _documentSize = size;
        _needUpdate = true;
    }
}

void    PlainTextFormater::SetAlignment(Alignment align)
{
    if (_alignment != align)
    {
        _alignment = align;
        _needUpdate = true;
    }
}

void    PlainTextFormater::InitDrawables(DrawableArray &drawableArray)
{
    // there is 2 drawables, one for the text and the other one for the underlined style.
    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo1 = new GL::GeometryBuffer<DefaultVertexType::Textured2d,false>(GL::Enum::Triangles);
    geo1->VBO().Init();// init the buffer to force it's creation and avoid it's done while we update the geometry

    MaterialConfig *conf1 = new MaterialConfig(GL::Blend::Alpha);
    conf1->Textures.InitSize(1);
    conf1->Textures[0] = _font->Bitmap();

    drawableArray.resize((_style.Enabled(Underlined)) ? 2 : 1);
    drawableArray[0] = new Drawable(geo1, conf1);

    if (_style.Enabled(Underlined))
    {
        GL::GeometryBuffer<DefaultVertexType::Colored2d,false> *geo2 = new GL::GeometryBuffer<DefaultVertexType::Colored2d,false>(GL::Enum::Triangles);
        geo2->VBO().Init();// init the buffer to force it's creation and avoid it's done while we update the geometry
        drawableArray[1] = new Drawable(geo2, new MaterialConfig());
    }
    _needUpdate = true;
}

void    PlainTextFormater::ComputeDrawables(Vector2f &textSize, DrawableArray &drawableArray, const Utils::Unicode::UTF32 &text)
{
    // Set the scaling factor to get the actual size
    std::size_t         textCharSize = text.size();
    float               baseSize = _font->BaseSize();
    float               factor   = _charSize / baseSize;
    unsigned int        nbVertices = 6 * textCharSize;
    unsigned int        noVertice = 0, noUnderline = 0;
    float               thickness = 0;

    // build the vertice tabs with an estimated maximum size.
    if (_style.Enabled(Bold))
        nbVertices += 6 * 4 * textCharSize;

    Array<DefaultVertexType::Textured2d>      vertices(nbVertices);
    Array<DefaultVertexType::Colored2d>       underlines;

    if (_style.Enabled(Underlined))
    {
        if (_documentSize == 0)
            underlines.InitSize((text.CharCount('\n') + 1) * 6);
        else
            underlines.InitSize(((text.CharCount('\n') + 1) * 6) + ((textCharSize * _charSize) / _documentSize));
        thickness = (_style.Enabled(Bold)) ? 3.f : 2.f;
    }

	// Initialize the rendering coordinates
    float           X = 0.f, Y = 0;
    float           width = 0, curHeight = 0;
    float           posOffsetLastLine = 0, curWordWidth = 0;
    float           sizeBetweenWords = 0, lastSizeBetweenWords = 0;
    float           curCharWidth = 0;
    float           italicCoeff = (_style.Enabled(Italic)) ? 0.208f : 0.f; // 12 degrees
    unsigned int    indexWordBegin = 0, indexLineBegin = 0;
    bool            endWord = false;
    bool            endLine = false;

    for (std::size_t i = 0; i < textCharSize; ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32              curChar = text[i];
        const Core::Glyph   *curGlyph = _font->GetGlyph(curChar);

        // Handle special characters
        if (curChar == L' ')
        {
            curCharWidth = ((curGlyph) ? curGlyph->Add * factor : _charSize);
            X += curCharWidth;
            sizeBetweenWords += curCharWidth;
            endWord = true;
        }
        else if (curChar == L'\t')
        {
            curCharWidth = ((curGlyph) ? (curGlyph->Add * factor * 4) : (_charSize * 4));
            X += curCharWidth;
            sizeBetweenWords += curCharWidth;
            endWord = true;
        }
        else if (curChar == L'\n')
        {
            lastSizeBetweenWords = sizeBetweenWords;
            curCharWidth = 0;
            endWord = true;
            endLine = true;
        }
        else if (curGlyph != NULL)
        {
            curCharWidth = curGlyph->Add * factor;
            sizeBetweenWords = 0;
        }

        // manage the alignment if we are at the end of a word
        ManageAlignment(endWord, endLine, X, Y, thickness, curCharWidth, curWordWidth, sizeBetweenWords, lastSizeBetweenWords,
                        indexLineBegin, indexWordBegin, posOffsetLastLine,
                        vertices, noVertice, underlines, noUnderline);

        if (endWord)
        {
            lastSizeBetweenWords = sizeBetweenWords;
            endWord = false;
            curGlyph = NULL;
        }

        if (curGlyph != NULL)
        {
            // we compute the offset of the last line to add to the matrix so that the rendering will match exactly with the box size of the string.
            if (posOffsetLastLine < (curGlyph->Pos.data[1] * factor))
                posOffsetLastLine = curGlyph->Pos.data[1] * factor;

            DrawVertices(vertices, noVertice, X, Y, thickness, italicCoeff, curGlyph, factor);

            // Advance to the next character
            X += curCharWidth;
            curWordWidth += curCharWidth;
            if (width < X)
                width = X;
            float charHeight = (float)(curGlyph->Size.data[1] + curGlyph->Pos.data[1]) * factor;
            if (Math::Abs(charHeight) > Math::Abs(curHeight))
                curHeight = charHeight;
            curCharWidth = 0;
        }
    }

    // manage alignment for last line
    endWord = true;
    endLine = true;
    ManageAlignment(endWord, endLine, X, Y, thickness, curCharWidth, curWordWidth, sizeBetweenWords, lastSizeBetweenWords,
                    indexLineBegin, indexWordBegin, posOffsetLastLine,
                    vertices, noVertice, underlines, noUnderline);
    Y += _charSize;

    // Add the last line (which was not finished with a \n)
    if (_style.Enabled(Underlined))
    {
        if (posOffsetLastLine < thickness)
            posOffsetLastLine = thickness;
    }


    // update the geometry
    // translate every caracter to it's final position (according to the computed size)
    TranslateCaraters(vertices.data, noVertice, 0.f, -Y + posOffsetLastLine);
    vertices.UnderSize(noVertice); // resize the buffer, to be sure that we render the good number of vertices
    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d,false>*>(drawableArray[0]->Geometry);
    geo->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);

    if (_style.Enabled(Underlined))
    {
        // translate every underline to it's final position (according to the computed size)
        TranslateUnderlines(underlines.data, noUnderline, 0.f, -Y + posOffsetLastLine);
        underlines.UnderSize(noUnderline); // resize the buffer, to be sure that we render the good number of vertices
        GL::GeometryBuffer<DefaultVertexType::Colored2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(drawableArray[1]->Geometry);
        geo->VBO().UpdateData(underlines, GL::Enum::DataBuffer::StaticDraw);
    }

    if (_documentSize > 0)
        width = _documentSize;
    textSize = Vector2f(width,  -Y + posOffsetLastLine - curHeight);
    _needUpdate = false;
}


void    PlainTextFormater::DrawUnderlines(Array<DefaultVertexType::Colored2d> &underlines, unsigned int &noUnderline, float X, float Y, float thickness)
{
// If we're using the underlined style and there's a new line,
// we keep track of the previous line to draw it later

    // check the size of the underlines tab... we double the size of the tab if that's too small.
    if (underlines.Size() < (noUnderline + 6))
        underlines.Resize(noUnderline * 2);

    underlines.data[noUnderline++].Fill(0, Y - 2, _color);
    underlines.data[noUnderline++].Fill(0, Y - 2 + thickness, _color);
    underlines.data[noUnderline++].Fill(X, Y - 2 + thickness, _color);

    underlines.data[noUnderline++].Fill(X, Y - 2 + thickness, _color);
    underlines.data[noUnderline++].Fill(X, Y - 2, _color);
    underlines.data[noUnderline++].Fill(0, Y - 2, _color);
}

void    PlainTextFormater::DrawVertices(Array<DefaultVertexType::Textured2d> &vertices, unsigned int &noVertice, float X, float Y, float thickness, float italicCoeff, const Glyph *curGlyph, float factor)
{
    float glyphPosX = curGlyph->Pos.data[0] * factor;
    float glyphPosY = curGlyph->Pos.data[1] * factor;
    float glyphSizeX = curGlyph->Size.data[0] * factor;
    float glyphSizeY = curGlyph->Size.data[1] * factor;

    // Draw a textured quad for the current character
    // first triangle
    vertices.data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)),
                                    Y - glyphPosY - glyphSizeY,
                                    curGlyph->Coord.Max(0),
                                    curGlyph->Coord.Min(1),
                                    _color);
    vertices.data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Max(0),
                                    curGlyph->Coord.Max(1),
                                    _color);
    vertices.data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Max(1),
                                    _color);

    // second triangle
    vertices.data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Max(1),
                                    _color);
    vertices.data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY - glyphSizeY)),
                                    Y - glyphPosY - glyphSizeY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Min(1),
                                    _color);
    vertices.data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)),
                                    Y - glyphPosY - glyphSizeY,
                                    curGlyph->Coord.Max(0),
                                    curGlyph->Coord.Min(1),
                                    _color);

    // If we're using the bold style, we must render the character 4 more times,
    // slightly offseted, to simulate a higher weight
    if (_style.Enabled(Bold))
    {
        static const float OffsetsX[] = {-0.5f, 0.5f, 0.f, 0.f};
        static const float OffsetsY[] = {0.f, 0.f, -0.5f, 0.5f};
        for (int j = 0; j < 4; ++j)
        {
            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Max(1), _color);
            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);

            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);
            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Min(1), _color);
            vertices.data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
        }
    }
}

void    PlainTextFormater::TranslateCaraters(DefaultVertexType::Textured2d *vertices, unsigned int noVertice, float offsetX, float offsetY)
{
    for (unsigned int i = 0; i < noVertice; ++i)
    {
        vertices[i].coord[0] += offsetX;
        vertices[i].coord[1] += offsetY;
    }
}

void    PlainTextFormater::TranslateUnderlines(DefaultVertexType::Colored2d *underlines, unsigned int noUnderline, float offsetX, float offsetY)
{
    for (unsigned int i = 0; i < noUnderline; ++i)
    {
        underlines[i].coord[0] += offsetX;
        underlines[i].coord[1] += offsetY;
    }
}

void    PlainTextFormater::ManageAlignment(bool &endWord, bool &endLine, float &X, float &Y, float thickness, float &curCharWidth, float &curWordWidth, float &sizeBetweenWords, float &lastSizeBetweenWords,
                                            unsigned int &indexLineBegin, unsigned int &indexWordBegin, float &posOffsetLastLine,
                                            Array<DefaultVertexType::Textured2d> &vertices, unsigned int &noVertice,
                                            Array<DefaultVertexType::Colored2d> &underlines, unsigned int &noUnderline)
{
    bool wordTooLong = false;

    // if the word is too long, treat the next as a new word
    if (!endWord && (_documentSize > 0 && (X + curCharWidth) > _documentSize))
    {
        wordTooLong = true;
        endWord = true;
        if (X == curWordWidth)
        {
            indexWordBegin = noVertice;
            curWordWidth = 0;
            lastSizeBetweenWords = 0;
        }
    }

    if (endWord)
    {
        if (endLine || (_documentSize > 0 && (X + curCharWidth - sizeBetweenWords) > _documentSize))
        {
            if (_documentSize == 0)
            {
                if (_style.Enabled(Underlined))
                    DrawUnderlines(underlines, noUnderline, X - lastSizeBetweenWords, Y, thickness);
                X = 0;
            }
            else
            {
                float lineWidth = X - sizeBetweenWords;
                if ((X + curCharWidth - sizeBetweenWords) > _documentSize)
                {
                    lineWidth -= (curWordWidth + lastSizeBetweenWords);

                    // translate the word to the next line
                    TranslateCaraters(vertices.data + indexWordBegin, noVertice - indexWordBegin,
                                      -X + sizeBetweenWords + curWordWidth, -_charSize);
                    X = curWordWidth + sizeBetweenWords;
                    endLine = true;
                }
                else
                {
                    X = 0;
                    indexWordBegin = noVertice;
                    endLine = false;
                }

                if (_style.Enabled(Underlined))
                    DrawUnderlines(underlines, noUnderline, lineWidth, Y, thickness);

                float translateLine = 0;
                if (_alignment == Center)
                    translateLine = (_documentSize - lineWidth) / 2;
                else if (_alignment == Right)
                    translateLine = (_documentSize - lineWidth);

                if (translateLine > 0)
                {
                    TranslateCaraters(vertices.data + indexLineBegin, (noVertice - indexLineBegin) - (noVertice - indexWordBegin), translateLine, 0);
                    TranslateUnderlines(underlines.data + (noUnderline - 6), 6, translateLine, 0);
                }
            }

            Y -= _charSize;
            posOffsetLastLine = 0;
            indexLineBegin = indexWordBegin;
            endLine = false;
        }
        if (wordTooLong == 0)
        {
            curWordWidth = 0;
            indexWordBegin = noVertice;
        }
    }

    if (wordTooLong != 0)
        endWord = false;
}
