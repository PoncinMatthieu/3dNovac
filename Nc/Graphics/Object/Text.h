
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

#ifndef NC_GRAPHICS_OBJECT_TEXT_H_
#define NC_GRAPHICS_OBJECT_TEXT_H_

#include "../Data/PlainTextFormater.h"
#include "Object.h"


namespace Nc
{
    namespace Graphic
    {
        /// Text class used to render text by using the given or default ITextFormater.
        /**
            To construct a text, you can either create a text formater separately and give it to the constructor (the formater will be deleted with the string).
            Or you can construct the text by setting up a charSize, a color, a ttf file, a style... Those parameters will be used to create the default formater: Core::PlainTextFormater.

            \sa
                - Core::ITextFormater.
                - Core::PlainTextFormater.
        */
        class LGRAPHICS   Text : public Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::Object, System::Object, Nc::Graphic::Text);

            public:
                /** Constructor with an instance of textFormater, the textFormater will be deleted in the destructor. */
                Text(const Utils::Unicode::UTF32 &text, ITextFormater *textFormater);
                /** Constructor used to create the text with a PlainTextFormater. */
                Text(const Utils::Unicode::UTF32 &text, float charSize, const Color &color, const std::string &ttf, const Utils::Mask<PlainTextFormater::Style> &s = PlainTextFormater::Regular);
                Text(const Text &text);
                Text &operator = (const Text &text);
                virtual ~Text();

                virtual ISceneNode              *Clone() const      {return new Text(*this);}

                /** \return the UTF32 plain text. */
                const Utils::Unicode::UTF32     &PlainText() const  {return _text;}
                /** Set the UTF32 plain text. */
                void                            PlainText(const Utils::Unicode::UTF32 &text);

                /** \return the text formater. */
                ITextFormater                   *Formater() const   {return _textFormater;}
                /** Set the text formater. */
                void                            Formater(ITextFormater *f);

                /** \return the size of the string. Will compute the size by using the textFormater if the text has changed. */
                const Vector2f      &Size();

            protected:
                /** Render the text. Recompute the drawables if needed. */
                virtual void        Render(SceneGraph *scene);

                virtual void        TransformModelMatrixToRender(SceneGraph *scene);

            protected:
                Utils::Unicode::UTF32   _text;              ///< store the plain text.
                ITextFormater           *_textFormater;     ///< the text formater used to compute the size and the drawables.
                Vector2f                _size;              ///< the size of the string (computed with RecomputeSize).

                System::Mutex           _mutex;             ///< the mutex used to protect the string.
        };
    }
}

#endif
