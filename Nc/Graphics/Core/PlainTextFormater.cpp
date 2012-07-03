
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
        _drawablesChanged = true;
    }
}

void    PlainTextFormater::SetCharSize(float size)
{
    if (_charSize != size)
    {
        _charSize = size;
        _drawablesChanged = _sizeChanged = true;
    }
}

Vector2f PlainTextFormater::GetCharSize(UInt32 c) const
{
    float           factor = _charSize / _font->BaseSize();
    const Glyph     *r = _font->GetGlyph(c);
    return (r != NULL) ? Vector2f(r->Size.Data[0] * factor, r->Size.Data[1] * factor) : Vector2f(0, _charSize);
}

void    PlainTextFormater::SetDocumentSize(float size)
{
    if (_documentSize != size)
    {
        _documentSize = size;
        _drawablesChanged = _sizeChanged = true;
    }
}

void    PlainTextFormater::SetAlignment(Alignment align)
{
    if (_alignment != align)
    {
        _alignment = align;
        _drawablesChanged = true;
    }
}

void    PlainTextFormater::ComputeSize(Vector2f &textSize, const Utils::Unicode::UTF32 &text)
{
    // No text, empty box :)
    if (text.empty())
    {
        textSize = Vector2f(0, _charSize);
        return;
    }

    // Initial values
    float curWidth = 0, curHeight = 0, width = 0, height = 0;
    float factor    = _charSize / (float)_font->BaseSize();

    // Go through each character
    std::size_t textSizeCar = text.size();
    for (std::size_t i = 0; i < textSizeCar; ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32          curChar  = text[i];
        const Glyph     *curGlyph = _font->GetGlyph(curChar);

        // Handle special characters
        switch (curChar)
        {
            case L' ' :
                curWidth += (curGlyph) ? (curGlyph->Add * factor) : _charSize;
                break;
            case L'\t' :
                curWidth += (curGlyph) ? (curGlyph->Add * factor * 4) : (_charSize * 4);
                break;
            case L'\n' :
                height += _charSize;
                if (curWidth > width)
                    width = curWidth;
                curWidth = 0;
                break;
            default: // every other caracters.
                if (curGlyph != NULL)
                    curWidth += curGlyph->Add * factor;
                break;
        }

        if (_documentSize > 0)
        {
            if (_alignment == Left)
            {
                if (curWidth > _documentSize)
                {
                    height += _charSize;
                    curWidth = (curGlyph != NULL) ? curGlyph->Add * factor : 0;
                }
            }
        }

        // Update the maximum height
        if (curGlyph != NULL)
        {
            float charHeight = (curGlyph->Size.Data[1] - curGlyph->Pos.Data[1]) * factor;
            if (Math::Abs(charHeight) > Math::Abs(curHeight))
                curHeight = charHeight;
        }
    }

    // Update the last line
    if (curWidth > width)
        width = curWidth;
    height += Math::Abs(curHeight);

    // Add a slight width / height if we're using the bold style
    if (_style.Enabled(Bold))
    {
        width  += 1 * factor;
        height += 1 * factor;
    }

    // Add a slight width if we're using the italic style
    if (_style.Enabled(Italic))
        width += 0.208f * _charSize;

    // Add a slight height if we're using the underlined style
    if ((_style.Enabled(Underlined)) && (Math::Abs(curHeight) < _charSize + 4 * factor))
        height += 4 * factor;

    if (_documentSize > 0)
    {
        if (_alignment == Left)
        {
            width = _documentSize;
        }
    }

    // Finally update the size
    textSize = Vector2f(width, Math::Abs(height));
    _sizeChanged = false;
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
    TextChanged();
}

void    PlainTextFormater::ComputeDrawables(DrawableArray &drawableArray, const Utils::Unicode::UTF32 &text)
{
    // Set the scaling factor to get the actual size
    std::size_t         textSize = text.size();
    float               baseSize = _font->BaseSize();
    float               factor   = _charSize / baseSize;
    unsigned int        nbVertices = 6 * textSize;
    unsigned int        noVertice = 0, noUnderline = 0;
    float               thickness = 0;

    // build the vertice tabs with an estimated maximum size.
    if (_style.Enabled(Bold))
        nbVertices += 6 * 4 * textSize;

    Array<DefaultVertexType::Textured2d>      vertices(nbVertices);
    Array<DefaultVertexType::Colored2d>       underlines;

    if (_style.Enabled(Underlined))
    {
        if (_documentSize == 0)
            underlines.InitSize((text.CharCount('\n') + 1) * 6);
        else
            underlines.InitSize(((text.CharCount('\n') + 1) * 6) + ((textSize * _charSize) / _documentSize));
        thickness = (_style.Enabled(Bold)) ? 3.f : 2.f;
    }

	// Initialize the rendering coordinates
    float X = 0.f, Y = _charSize, posOffsetLastLine = 0;
    float italicCoeff = (_style.Enabled(Italic)) ? 0.208f : 0.f; // 12 degrees

    for (std::size_t i = 0; i < textSize; ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32              curChar = text[i];
        const Core::Glyph   *curGlyph = _font->GetGlyph(curChar);

        // Handle special characters
        if (curChar == L' ')
        {
            X += (curGlyph) ? curGlyph->Add * factor : _charSize;
            continue;
        }
        else if (curChar == L'\t')
        {
            X += (curGlyph) ? (curGlyph->Add * factor * 4) : (_charSize * 4);
            continue;
        }
        else if (curChar == L'\n')
        {
            if (_style.Enabled(Underlined))
                DrawUnderlines(underlines, noUnderline, X, Y, thickness);
            Y -= _charSize;
            X = 0;
            posOffsetLastLine = 0;
            continue;
        }
        else if (curGlyph == NULL)  // move away, if the glyph is null
            continue;

        if (_documentSize > 0)
        {
            if (_alignment == Left)
            {
                if ((X + ((curGlyph->Pos.Data[0] + curGlyph->Size.Data[0]) * factor)) > _documentSize)
                {
                    if (_style.Enabled(Underlined))
                        DrawUnderlines(underlines, noUnderline, X, Y, thickness);
                    Y -= _charSize;
                    X = 0;
                    posOffsetLastLine = 0;
                }
            }
        }

        // we compute the offset of the last line to add to the matrix so that the rendering will match exactly with the box size of the string.
        if (posOffsetLastLine < (curGlyph->Pos.Data[1] * factor))
            posOffsetLastLine = curGlyph->Pos.Data[1] * factor;

        DrawVertices(vertices, noVertice, X, Y, thickness, italicCoeff, curGlyph, factor);

        // Advance to the next character
        X += curGlyph->Add * factor;
    }

    // Add the last line (which was not finished with a \n)
    if (_style.Enabled(Underlined))
        DrawUnderlines(underlines, noUnderline, X, Y, thickness);

    // translate every caracter to it's final position (according to the computed size)
    TranslateCaraters(vertices, noVertice, underlines, noUnderline, 0.f, -Y + posOffsetLastLine);

    // update the geometry
    vertices.UnderSize(noVertice); // resize the buffer, to be sure that we render the good number of vertices
    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d,false>*>(drawableArray[0]->Geometry);
    geo->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);

    if (_style.Enabled(Underlined))
    {
        underlines.UnderSize(noUnderline); // resize the buffer, to be sure that we render the good number of vertices
        GL::GeometryBuffer<DefaultVertexType::Colored2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(drawableArray[1]->Geometry);
        geo->VBO().UpdateData(underlines, GL::Enum::DataBuffer::StaticDraw);
    }
    _drawablesChanged = false;
}


void    PlainTextFormater::DrawUnderlines(Array<DefaultVertexType::Colored2d> &underlines, unsigned int &noUnderline, float X, float Y, float thickness)
{
// If we're using the underlined style and there's a new line,
// we keep track of the previous line to draw it later

    // check the size of the underlines tab... we double the size of the tab if that's too small.
    if (underlines.Size() < (noUnderline + 6))
        underlines.Resize(noUnderline * 2);

    underlines.Data[noUnderline++].Fill(0, Y - 2, _color);
    underlines.Data[noUnderline++].Fill(0, Y - 2 + thickness, _color);
    underlines.Data[noUnderline++].Fill(X, Y - 2 + thickness, _color);

    underlines.Data[noUnderline++].Fill(X, Y - 2 + thickness, _color);
    underlines.Data[noUnderline++].Fill(X, Y - 2, _color);
    underlines.Data[noUnderline++].Fill(0, Y - 2, _color);
}

void    PlainTextFormater::DrawVertices(Array<DefaultVertexType::Textured2d> &vertices, unsigned int &noVertice, float X, float Y, float thickness, float italicCoeff, const Core::Glyph *curGlyph, float factor)
{
    float glyphPosX = curGlyph->Pos.Data[0] * factor;
    float glyphPosY = curGlyph->Pos.Data[1] * factor;
    float glyphSizeX = curGlyph->Size.Data[0] * factor;
    float glyphSizeY = curGlyph->Size.Data[1] * factor;

    // Draw a textured quad for the current character
    // first triangle
    vertices.Data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)),
                                    Y - glyphPosY - glyphSizeY,
                                    curGlyph->Coord.Max(0),
                                    curGlyph->Coord.Min(1),
                                    _color);
    vertices.Data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Max(0),
                                    curGlyph->Coord.Max(1),
                                    _color);
    vertices.Data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Max(1),
                                    _color);

    // second triangle
    vertices.Data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY)),
                                    Y - glyphPosY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Max(1),
                                    _color);
    vertices.Data[noVertice++].Fill(X + glyphPosX + (italicCoeff * (-glyphPosY - glyphSizeY)),
                                    Y - glyphPosY - glyphSizeY,
                                    curGlyph->Coord.Min(0),
                                    curGlyph->Coord.Min(1),
                                    _color);
    vertices.Data[noVertice++].Fill(X + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)),
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
            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Max(1), _color);
            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);

            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY)), Y + OffsetsY[j] - glyphPosY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);
            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Min(0), curGlyph->Coord.Min(1), _color);
            vertices.Data[noVertice++].Fill(X + OffsetsX[j] + glyphPosX + glyphSizeX + (italicCoeff * (-glyphPosY - glyphSizeY)), Y + OffsetsY[j] - glyphPosY - glyphSizeY,
                                            curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
        }
    }
}

void    PlainTextFormater::TranslateCaraters(Array<Core::DefaultVertexType::Textured2d> &vertices, unsigned int noVertice,
                                             Array<Core::DefaultVertexType::Colored2d> &underlines, unsigned int noUnderline, float offsetX, float offsetY)
{
    for (unsigned int i = 0; i < noVertice; ++i)
    {
        vertices.Data[i].coord[0] += offsetX;
        vertices.Data[i].coord[1] += offsetY;
    }

    for (unsigned int i = 0; i < noUnderline; ++i)
    {
        underlines.Data[i].coord[0] += offsetX;
        underlines.Data[i].coord[1] += offsetY;
    }
}
