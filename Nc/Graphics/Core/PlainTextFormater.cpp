
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
#include "DefaultVertexType.h"
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
    for (size_t i = 0; i < text.size(); ++i)
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

void    PlainTextFormater::ComputeDrawables(DrawableArray &drawableArray, TMatrix &matrix, const Utils::Unicode::UTF32 &text)
{
    // Set the scaling factor to get the actual size
    float               baseSize = _font->BaseSize();
    float               factor   = _charSize / baseSize;
    unsigned int        nbVertices = 6 * text.size(), noVertice = 0, noBackSlashN = 0;
    float               thickness = 0;

    if (_style.Enabled(Bold))
        nbVertices += 6 * 4 * text.size();

    // construit les arrays de vertices
    Array<DefaultVertexType::Textured2d>      vertices(nbVertices);
    Array<DefaultVertexType::Colored2d>       underlines;

    // Holds the lines to draw later, for underlined style
    if (_style.Enabled(Underlined))
    {
        underlines.InitSize((text.CharCount('\n') + 1) * 6);
        thickness = (_style.Enabled(Bold)) ? 3.f : 2.f;
    }

	// Initialize the rendering coordinates
    float X = 0.f, Y = baseSize, posOffsetLastLine = 0;

    // Compute the shearing to apply if we're using the italic style
    float italicCoeff = (_style.Enabled(Italic)) ? 0.208f : 0.f; // 12 degrees

    // Draw one quad for each character
    for (std::size_t i = 0; i < text.size(); ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32              curChar = text[i];
        const Core::Glyph   *curGlyph = _font->GetGlyph(curChar);


        // If we're using the underlined style and there's a new line,
        // we keep track of the previous line to draw it later
        if ((curChar == L'\n') && (_style.Enabled(Underlined)))
        {
            underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
            underlines.Data[noBackSlashN++].Fill(0, Y - 2 + thickness, _color);
            underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);

            underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);
            underlines.Data[noBackSlashN++].Fill(X, Y - 2, _color);
            underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
        }

        // Handle special characters
        if (curChar == L' ')
        {
            X += (curGlyph) ? curGlyph->Add : baseSize;
            continue;
        }
        else if (curChar == L'\t')
        {
            X += (curGlyph) ? (curGlyph->Add * 4) : (baseSize * 4);
            continue;
        }
        else if (curChar == L'\n')
        {
            Y -= baseSize;
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
                if (((X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0]) * factor) > _documentSize)
                {
                    Y -= baseSize;
                    X = 0;
                    posOffsetLastLine = 0;
                }
            }
        }

        // we compute the offset of the last line to add to the matrix so that the rendering will match exactly with the box size of the string.
        if (posOffsetLastLine < curGlyph->Pos.Data[1])
            posOffsetLastLine = curGlyph->Pos.Data[1];

        // Draw a textured quad for the current character
        // first triangle
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])),
                                        Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                        curGlyph->Coord.Max(0),
                                        curGlyph->Coord.Min(1),
                                        _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])),
                                        Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Max(0),
                                        curGlyph->Coord.Max(1),
                                        _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])),
                                        Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Min(0),
                                        curGlyph->Coord.Max(1),
                                        _color);

        // second triangle
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])),
                                        Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Min(0),
                                        curGlyph->Coord.Max(1),
                                        _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])),
                                        Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                        curGlyph->Coord.Min(0),
                                        curGlyph->Coord.Min(1),
                                        _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])),
                                        Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
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
                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                                curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1],
                                                curGlyph->Coord.Max(0), curGlyph->Coord.Max(1), _color);
                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1],
                                                curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);

                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1],
                                                curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);
                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                                curGlyph->Coord.Min(0), curGlyph->Coord.Min(1), _color);
                vertices.Data[noVertice++].Fill(X + OffsetsX[j] + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y + OffsetsY[j] - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                                curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
            }
        }

        X += curGlyph->Add;        // Advance to the next character
    }

    // setup the matrix so the rendering will match the box size
    matrix.Translation(0.f, -Y + posOffsetLastLine, 0.f);
    matrix.AddScale(factor, factor, 1.f);

    // Add the last line (which was not finished with a \n)
    if (_style.Enabled(Underlined))
    {
        underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
        underlines.Data[noBackSlashN++].Fill(0, Y - 2 + thickness, _color);
        underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);

        underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);
        underlines.Data[noBackSlashN++].Fill(X, Y - 2, _color);
        underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);

        underlines.UnderSize(noBackSlashN); // resize the buffer, to be sure that we render the good number of vertices
        GL::GeometryBuffer<DefaultVertexType::Colored2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d,false>*>(drawableArray[1]->Geometry);
        geo->VBO().UpdateData(underlines, GL::Enum::DataBuffer::StaticDraw);
    }

    vertices.UnderSize(noVertice); // resize the buffer, to be sure that we render the good number of vertices
    GL::GeometryBuffer<DefaultVertexType::Textured2d,false> *geo = static_cast<GL::GeometryBuffer<DefaultVertexType::Textured2d,false>*>(drawableArray[0]->Geometry);
    geo->VBO().UpdateData(vertices, GL::Enum::DataBuffer::StaticDraw);
    _drawablesChanged = false;
}
