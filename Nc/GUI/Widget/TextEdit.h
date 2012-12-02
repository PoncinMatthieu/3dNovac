
/*-----------------------------------------------------------------------------

	3dNovac GUI
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

    File Created At:        30/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_TEXTEDIT_H_
#define NC_GUI_WIDGET_TEXTEDIT_H_

#include "ScrollArea.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a textEdit Widget.
        /**
            The textEdit Widget defines a text area and allow it's management / rendering into a ScrollArea.

            \todo Make the text edit editable.
        */
        class LGUI TextEdit : public ScrollArea
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(ScrollArea, System::Object, Nc::GUI::TextEdit);

            private:
                /// Inner class defining a text view to be displayed into the text edit scroll area.
                struct TextDocument : public Widget
                {
                    NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::TextEdit::TextDocument);

                    TextDocument(TextEdit *editor, const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment, const Vector2i &size, const std::string &ttf, const Utils::Mask<Graphic::PlainTextFormater::Style> &s);
                    TextDocument(const TextDocument &textDocument);
                    TextDocument &operator = (const TextDocument &textDocument);
                    virtual ~TextDocument();

                    virtual ISceneNode      *Clone() const          {return new TextDocument(*this);}

                    /** Resize the geometry of the text. */
                    virtual void        Resize();
                    /** Update the geometry of the text. */
                    virtual void        UpdateState();
                    /** Render the text area. */
                    virtual void        Draw(Graphic::SceneGraph *scene);

                    Graphic::Text       *text;
                    TextEdit            *_editor;
                };

            public:
                TextEdit(const Utils::Unicode::UTF32 &text, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const std::string &ttf = "arial", const Utils::Mask<Graphic::PlainTextFormater::Style> &s = Graphic::PlainTextFormater::Regular);
                TextEdit(const TextEdit &edit);
                TextEdit &operator = (const TextEdit &edit);
                virtual ~TextEdit();

                virtual ISceneNode              *Clone() const          {return new TextEdit(*this);}

                /** \return the text. */
                const Utils::Unicode::UTF32     &PlainText() const      {return _textDocument->text->PlainText();}
                /** Set the text. */
                void                            PlainText(const Utils::Unicode::UTF32 &t);

            private:
                /** Render the textArea before disabling the stencil. */
                virtual void        RenderChildsEnd(Graphic::SceneGraph *scene);

                void                Copy(const TextEdit &edit);

            private:
                TextDocument    *_textDocument;     ///< text document used to display the text.
        };
    }
}

#endif
