
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

    File Created At:        03/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_TEXTBOX_H_
#define NC_GUI_WIDGET_TEXTBOX_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a lineEdit Widget.
        /**
            The line edit allow to enter text into an single line text field.

            - Send an ReturnPressed event when the Return/Enter key is pressed.
            - Send a TextEntered event whenever the text changed from a keyboard event.
        */
        class LIB_NC_GUI  LineEdit : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::LineEdit);

            public:
                LineEdit(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0), const std::string &ttf = "arial", const std::string &looksName = "");
                LineEdit(const LineEdit &w);
                LineEdit &operator = (const LineEdit &w);
                ~LineEdit();

                virtual ISceneNode  *Clone() const              {return new LineEdit(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Set the text of the line edit. */
                void                    PlainText(const Utils::Unicode::UTF32 &t);
                /** \return the text of the line edit. */
                Utils::Unicode::UTF32   PlainText() const;

            protected:
                /** update the geometry of the line edit. */
                virtual void UpdateState();
                /** Render the line edit. */
                virtual void Draw(Graphic::SceneGraph *scene);

                /** the keyboard handler. */
                virtual void KeyboardEvent(const System::Event &event);

            private:
                /** Copy the widget. */
                void        Copy(const LineEdit &w);

            protected:
                Graphic::Text       *_textLeft;                 ///< the text on the left of the cursor.
                Graphic::Text       *_textRight;                ///< the text on the right of the cursor.
                Graphic::Text       *_cursorText;               ///< to draw an underscore (cursor).
                float               _currentOffset;             ///< store the current offset used to display the text, in case the text is longer that the size of the box.
                bool                _editable;                  ///< if true, the linedit will receive events from the keyboard.
                bool                _displayCursor;             ///< if true, the underscore is displayed.
                Utils::Clock        _clock;                     ///< clock used to make the cursor blink.
        };
    }
}

#endif
