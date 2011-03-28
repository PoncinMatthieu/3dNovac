
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

#include "String.h"

using namespace std;
using namespace Nc::Graphic;
using namespace Nc;

String::MapFont         String::_mapFont;
System::Mutex           String::_mutex;

String::String(const Utils::Unicode::UTF32 &text, unsigned int size, const Color &color, const std::string &ttf, unsigned long style)
    : Object2d(), _material(&Material<BasicVertexType::Textured2d>::Instance()), _materialUnderline(&Material<BasicVertexType::Colored2d>::Instance())
{
    // search the font in the map
    MapFont::iterator it = _mapFont.find(ttf);
    if (it == _mapFont.end()) // si on ne la pas, on creer la font
    {
        Utils::FileName s = CONFIG->Block("RessourcesPath")->Line("Font")->Param("path") + ttf + ".ttf";
        _mapFont[ttf] = _font = new Font();
        _font->LoadFromFile(s, CHAR_SIZE);
    }
    else
        _font = it->second;

    _config.Blend = GL::Blend::Alpha;
    _config.Texture = _font->Bitmap();

    _text = text;
    _color = color;
    _style = style;
    _charSize = size;

    _geometry.GetVBO().Init(); // force la creation du buffer, pour eviter que ce soit fait dans l'update geometry
    _materialUnderline->Configure(_geometryUnderline);
    _material->Configure(_geometry);
    _needUpdate = _needUpdateSize = true;
}

String::~String()
{
}

void    String::Render(ISceneGraph *scene)
{
    if (!_text.empty())    // No text, no rendering :)
    {
        if (_needUpdate)
            UpdateGeometry();
        TMatrix m = scene->ModelMatrix() * Matrix * _matrixText;
        _material->Render(scene, m, _geometry, _config);
        if (_style & Underlined)
            _materialUnderline->Render(scene, m, _geometryUnderline, _configUnderline);
    }
}

Vector2f String::GetCharSize(char c) const
{
    float           factor = _charSize / _font->BaseSize();
    const Glyph     *r = _font->GetGlyph(c);
    return (r != NULL) ? Vector2f(r->Size.Data[0] * factor, r->Size.Data[1] * factor) : Vector2f(0, _charSize);
}

void String::UpdateGeometry()
{
    _mutex.Lock();
    _needUpdate = false;

    // Set the scaling factor to get the actual size
    float               baseSize = _font->BaseSize();
    float               factor   = _charSize / baseSize;
    unsigned int        nbVertices = 6 * _text.size(), noVertice = 0, noBackSlashN = 0;
    float               thickness = 0;

    if (_style & Bold)
        nbVertices += 6 * 4 * _text.size();

    // construit les arrays de vertices
    Array<BasicVertexType::Textured2d>      vertices(nbVertices);
    Array<BasicVertexType::Colored2d>       underlines;

    // Holds the lines to draw later, for underlined style
    if (_style & Underlined)
    {
        underlines.InitSize((_text.CharCount('\n') + 1) * 6);
        thickness = (_style & Bold) ? 3.f : 2.f;
    }

    _matrixText.Translation(0.f, -baseSize, 0.f);
    _matrixText.AddScale(factor, factor, 1.f);

	// Initialize the rendering coordinates
    float X = 0.f, Y = baseSize;

    // Compute the shearing to apply if we're using the italic style
    float italicCoeff = (_style & Italic) ? 0.208f : 0.f; // 12 degrees

    // Draw one quad for each character
    for (std::size_t i = 0; i < _text.size(); ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32          curChar = _text[i];
        const Glyph     *curGlyph = _font->GetGlyph(curChar);

        if (curGlyph == NULL)  // move away, if the glyph is null
            continue;

        // If we're using the underlined style and there's a new line,
        // we keep track of the previous line to draw it later
        if ((curChar == L'\n') && (_style & Underlined))
        {
            underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
            underlines.Data[noBackSlashN++].Fill(0, Y - 2 + thickness, _color);
            underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);

            underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);
            underlines.Data[noBackSlashN++].Fill(X, Y - 2, _color);
            underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
        }

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  X += curGlyph->Add;    break;
            case L'\n' : Y += baseSize; X = 0;  break;
            case L'\t' : X += (baseSize  * 4);  break;
            case L'\v' : Y += (baseSize * 4);   break;
        }

        // Draw a textured quad for the current character
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                        curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Max(0), curGlyph->Coord.Max(1), _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);

        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1])), Y - curGlyph->Pos.Data[1],
                                        curGlyph->Coord.Min(0), curGlyph->Coord.Max(1), _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                        curGlyph->Coord.Min(0), curGlyph->Coord.Min(1), _color);
        vertices.Data[noVertice++].Fill(X + curGlyph->Pos.Data[0] + curGlyph->Size.Data[0] + (italicCoeff * (-curGlyph->Pos.Data[1] - curGlyph->Size.Data[1])), Y - curGlyph->Pos.Data[1] - curGlyph->Size.Data[1],
                                        curGlyph->Coord.Max(0), curGlyph->Coord.Min(1), _color);

        // If we're using the bold style, we must render the character 4 more times,
        // slightly offseted, to simulate a higher weight
        if (_style & Bold)
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
    _mutex.Unlock();

    // Add the last line (which was not finished with a \n)
    if (_style & Underlined)
    {
        underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);
        underlines.Data[noBackSlashN++].Fill(0, Y - 2 + thickness, _color);
        underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);

        underlines.Data[noBackSlashN++].Fill(X, Y - 2 + thickness, _color);
        underlines.Data[noBackSlashN++].Fill(X, Y - 2, _color);
        underlines.Data[noBackSlashN++].Fill(0, Y - 2, _color);

        underlines.UnderSize(noBackSlashN); // resize the buffer, to be sure that we render the good number of vertices
        _geometryUnderline.GetVBO().UpdateData(underlines, GL_STATIC_DRAW);
    }

    vertices.UnderSize(noVertice); // resize the buffer, to be sure that we render the good number of vertices
    _geometry.GetVBO().UpdateData(vertices, GL_STATIC_DRAW);
}

void String::RecomputeRect()
{
    _mutex.Lock();

    // Reset the "need update" state
    _needUpdateSize = false;

    // No text, empty box :)
    if (_text.empty())
    {
        _size = Vector2f(0, _charSize);
        _mutex.Unlock();
        return;
    }

    // Initial values
    float curWidth = 0, curHeight = 0, width = 0, height = 0;
    float factor    = _charSize / (float)_font->BaseSize();

    // Go through each character
    for (size_t i = 0; i < _text.size(); ++i)
    {
        // Get the current character and its corresponding glyph
        UInt32          curChar  = _text[i];
        const Glyph     *curGlyph = _font->GetGlyph(curChar);

        if (curGlyph == NULL)  // move away, if the glyph is null
            continue;

        // Handle special characters
        switch (curChar)
        {
            case L' ' :  curWidth += curGlyph->Add * factor;            break;
            case L'\t' : curWidth += _charSize * 4;                     break;
            case L'\v' : height   += _charSize * 4; curHeight = 0;      break;
            case L'\n' :
                height += _charSize;
                curHeight = 0;
                if (curWidth > width)
                    width = curWidth;
                curWidth = 0;
                break;
        }

        // Advance to the next character
        curWidth += curGlyph->Add * factor;

        // Update the maximum height
        float charHeight = (curGlyph->Size.Data[1]) * factor;
        if (Math::Abs(charHeight) > Math::Abs(curHeight))
            curHeight = charHeight;
    }
    _mutex.Unlock();

    // Update the last line
    if (curWidth > width)
        width = curWidth;
    height += curHeight;

    // Add a slight width / height if we're using the bold style
    if (_style & Bold)
    {
        width  += 1 * factor;
        height += 1 * factor;
    }

    // Add a slight width if we're using the italic style
    if (_style & Italic)
        width += 0.208f * _charSize;

    // Add a slight height if we're using the underlined style
    if ((_style & Underlined) && (Math::Abs(curHeight) < _charSize + 4 * factor))
        height += 4 * factor;

    // Finally update the rectangle
    _size = Vector2f(width, Math::Abs(height));
}

