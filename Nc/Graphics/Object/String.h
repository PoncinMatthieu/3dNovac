
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

#ifndef NOVAC_GRAPHIC_2D_STRING_H_
#define NOVAC_GRAPHIC_2D_STRING_H_

#include <Nc/Core/Utils/Mask.h>
#include "../Core/Font.h"
#include "Object.h"

#define CHAR_SIZE   20.f

namespace Nc
{
    namespace Graphic
    {
        class Font;

        /// string Object2d class to render text
        /**
            A Graphical 2d string is defined by an UTF32 string, a size, a color, pattern style and a font
        */
        class LGRAPHICS   String : public Object
        {
            public:
                NC_UTILS_DEFINE_PARENT_CLASS(Graphic::Object);
                NC_UTILS_DEFINE_VISITABLE(System::Object);

                /** Define the style of the text */
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
                String(const Utils::Unicode::UTF32 &text, float size, const Color &color, const std::string &ttf, const Utils::Mask<Style> &s = Regular);
                virtual ~String();

                static const char               *ClassName()                                    {return "String";}
                virtual ISceneNode              *Clone() const                                  {return new String(*this);}

                /** Set the UTF32 text */
                void                            Text(const Utils::Unicode::UTF32 &text)         {_needUpdate = _needUpdateSize = true;   _text = text;}
                /** Return the UTF32 text */
                const Utils::Unicode::UTF32     &Text() const                                   {return _text;}

                /** Set the color */
                void                SetColor(const Color &color)        {_needUpdate = true; _color = color;}
                /** Return the size */
                const Vector2f      &Size()                             {if (_needUpdateSize) RecomputeSize(); return _size;}
                /** Return the size of the specified character */
                Vector2f            GetCharSize(UInt32 c) const;
                /** Return the global char size */
                float               CharSize() const                    {return _charSize;}
                /** Set the global char size */
                void                CharSize(float size)                {_charSize = size; _needUpdate = _needUpdateSize = true;}

                inline Font         *GetFont()                          {return _font;}

                /** Destroy the font, to call at the end of the program to avoid memory leek */
                static void         DestroyFonts();

            protected:
                /** Render the text */
                virtual void        Render(SceneGraph *scene);

                virtual void        TransformModelMatrixToRender(SceneGraph *scene);

            private:
                /** Update the geometry of the drawables used to render the text */
                void                UpdateGeometry();

                /** Recompute the size of the text, called when you will try to acces to the size */
                void                RecomputeSize();

                bool                _needUpdate;                ///< if true, update the geometry of the text
                bool                _needUpdateSize;            ///< if true, update the rendering size of the string

                Utils::Unicode::UTF32   _text;                  ///< the UTF32 string
                TMatrix                 _matrixText;            ///< the matrix placement of the text, used to scale it with the good char size
                Font                    *_font;                 ///< the instance of the used font
                Vector2f                _size;                  ///< the size of the string (computed with RecomputeSize)
                Utils::Mask<Style>      _style;                 ///< the pattern rendering style mask
                float                   _charSize;              ///< the global char size
                Color                   _color;                 ///< the color of the string

                static MapFont          _mapFont;               ///< the map of loaded font
                static System::Mutex    _mutex;                 ///< the mutex used to protect the string
        };
    }
}

#endif
