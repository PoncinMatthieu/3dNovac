
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

#ifndef NC_GRAPHICS_CORE_STRING_H_
#define NC_GRAPHICS_CORE_STRING_H_

#include <Nc/Core/Utils/Mask.h>
#include "Font.h"
#include "ITextFormater.h"
#include "DefaultVertexType.h"

/**
    \todo We create the font by using the following charSize. Find a better way to manage fonts and it's baseSize.
*/
#define CHAR_SIZE   20.f

namespace Nc
{
    namespace Graphic
    {
        namespace Core
        {
            /// PlainTextFormater used to plain text elements.
            /**
                The formater allow to render plain text with the following parameter:
                    - charSize: define the size of the caracters.
                    - style: define the style of the text (Bold, Underlined, etc...).
                    - color: define the color of the text.
                    - font: define the font of the text.
                    - alignment: define the alignment method (Left, Center, etc...). Use the document size to align the text.
                    - documentSize: limit the text horizontaly. If the document size is null, the text won't be limited.

                \todo We create the font into the PlainTextFormater and store it into a static map. Find a better way to create and store fonts.
                \todo the constructor takes the name of the font to retreive/create the font dynamically. Find a better way to create and store fonts.
                \todo implement Justify alignment.
            */
            class LGRAPHICS PlainTextFormater : public ITextFormater
            {
                public:
                    /** Define the style of the text. */
                    enum Style
                    {
                        Regular =       0,
                        Bold =          1 << 0,
                        Italic =        1 << 1,
                        Underlined =    1 << 2
                    };

                    /** Define an alignment method of the text. */
                    enum Alignment
                    {
                        Left,           ///< Align the text to the left.
                        Center,         ///< Center the text.
                        Right,          ///< Align the text to the right.
//                        Justify         ///< Align the text to both left and right, adding extra space between words as necessary.
                    };

                public:
                    PlainTextFormater(float charSize, const Color &color, const std::string &ttf, const Utils::Mask<Style> &s);
                    virtual ~PlainTextFormater();

                    virtual ITextFormater   *Clone() const              {return new PlainTextFormater(*this);}

                    /** Set the color. */
                    void                SetColor(const Color &color);
                    /** \return the color. */
                    const Color         &GetColor() const               {return _color;}

                    /** Return the size of the specified character. */
                    Vector2f            GetCharSize(UInt32 c) const;
                    /** Return the global char size. */
                    float               GetCharSize() const             {return _charSize;}
                    /** Set the global char size. */
                    void                SetCharSize(float size);

                    /** \return the font associated to the PlainTextFormater. */
                    inline Font         *GetFont() const                {return _font;}

                    /** \return the documentSize of the text. */
                    float               GetDocumentSize() const         {return _documentSize;}
                    /** Set documentSize, the text will be limited horizontally according to the alignment method. */
                    void                SetDocumentSize(float size);

                    /** \return the alignment method of the text. */
                    Alignment           GetAlignment() const            {return _alignment;}
                    /** Set the alignment method used according to the document size. */
                    void                SetAlignment(Alignment align);

                    /** Init drawables array, to avoid to recreate the drawables after each modification of the text */
                    virtual void        InitDrawables(DrawableArray &drawableArray);

                    /**
                        Create every Drawables for the given \p text and add them to the given \p drawableArray.
                        \return the computed size of the text.
                    */
                    virtual void        ComputeDrawables(Vector2f &textSize, DrawableArray &drawableArray, const Utils::Unicode::UTF32 &text);

                    /** Destroy the font, to call at the end of the program to avoid memory leak. */
                    static void         DestroyFonts();

                private:
                    void    DrawVertices(Array<Core::DefaultVertexType::Textured2d> &vertices, unsigned int &noVertice, float X, float Y, float thickness, float italicCoeff, const Core::Glyph *curGlyph, float factor);
                    void    DrawUnderlines(Array<Core::DefaultVertexType::Colored2d> &underlines, unsigned int &noUnderline, float X, float Y, float thickness);

                    void    TranslateCaraters(Core::DefaultVertexType::Textured2d *vertices, unsigned int noVertice, float offsetX, float offsetY);
                    void    TranslateUnderlines(Core::DefaultVertexType::Colored2d *underlines, unsigned int noUnderline, float offsetX, float offsetY);

                    void    ManageAlignment(bool &endWord, bool &newLine, float &X, float &Y, float thickness, float &curCharWidth, float &curWordWidth, float &sizeBetweenWords, float &lastCharSizeBetweenWords, unsigned int &indexLineBegin, unsigned int &indexWordBegin, float &posOffsetLastLine,
                                            Array<Core::DefaultVertexType::Textured2d> &vertices, unsigned int &noVertice,
                                            Array<Core::DefaultVertexType::Colored2d> &underlines, unsigned int &noUnderline);

                protected:
                    Font                    *_font;                 ///< the instance of the used font.
                    Utils::Mask<Style>      _style;                 ///< the pattern rendering style mask.
                    float                   _charSize;              ///< the global char size.
                    Color                   _color;                 ///< the color of the string.

                    float                   _documentSize;          ///< document size used to limit the horizontal size of the text. if null, the text won't be limited by any size.
                    Alignment               _alignment;             ///< describe how the text should be aligned according to the document size.

                    static FontMap          _mapFont;               ///< static map of loaded fonts.
            };
        }
    }
}

#endif
