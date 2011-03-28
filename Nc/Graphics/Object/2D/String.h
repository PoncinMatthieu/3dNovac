
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

    File Created At:        20/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------

    Implementation de la classe "String", pour afficher du text

--------------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_2D_STRING_H_
#define NOVAC_GRAPHIC_2D_STRING_H_

#include "../../Core/Font.h"
#include "Object2d.h"

#define CHAR_SIZE   20.f

namespace Nc
{
    namespace Graphic
    {
        class Font;

        class LGRAPHICS   String : public Object2d
        {
            public:
                enum Style
                {
                    Regular =       0,
                    Bold =          1 << 0,
                    Italic =        1 << 1,
                    Underlined =    1 << 2
                };

            private:
                typedef std::map<std::string, Font*>    MapFont;

            public:
                String(const Utils::Unicode::UTF32 &text, unsigned int size, const Color &color, const std::string &ttf, unsigned long s = Regular);
                virtual ~String();

                virtual void        Render(ISceneGraph *scene);
                void                UpdateGeometry();

                void                            Text(const Utils::Unicode::UTF32 &text)         {_needUpdate = _needUpdateSize = true;   _text = text;}
                const Utils::Unicode::UTF32     &Text() const                                   {return _text;}

                void                SetColor(const Color &color)        {_needUpdate = true; _color = color;}
                const Vector2f      &Size()                             {if (_needUpdateSize) RecomputeRect(); return _size;}
                Vector2f            GetCharSize(char c) const;
                float               CharSize() const                    {return _charSize;}
                void                CharSize(float size)                {_charSize = size; _needUpdate = _needUpdateSize = true;}

            private:
                void                RecomputeRect();

                Material<BasicVertexType::Textured2d>                       *_material;
                MaterialConfig<BasicVertexType::Textured2d>                 _config;
                GL::GeometryBuffer<BasicVertexType::Textured2d, false>      _geometry;

                Material<BasicVertexType::Colored2d>                        *_materialUnderline;
                GL::GeometryBuffer<BasicVertexType::Colored2d, false>       _geometryUnderline;
                MaterialConfig<BasicVertexType::Colored2d>                  _configUnderline;

                bool                _needUpdate;
                bool                _needUpdateSize;

                Utils::Unicode::UTF32   _text;
                TMatrix                 _matrixText;
                Font                    *_font;
                Vector2f                _size;
                unsigned long           _style;
                float                   _charSize;
                Color                   _color;

                static MapFont          _mapFont;
                static System::Mutex    _mutex;
        };
    }
}

#endif
