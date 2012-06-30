
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
        /// To manage a text box
        class LGUI  LineEdit : public Widget
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

                /** Set the text of the text box. */
                void                                Text(const Utils::Unicode::UTF32 &t)        {_font->Text(t);}
                /** Return the text of the text box. */
                inline const Utils::Unicode::UTF32  &Text() const                               {return _font->Text();}

            protected:
                /** update the geometry of the text box. */
                virtual void Update();
                /** Render the text box. */
                virtual void Draw(Graphic::SceneGraph *scene);

                /** the keyboard handler. */
                virtual void KeyboardEvent(const System::Event &event);

            private:
                /** Copy the widget. */
                void        Copy(const LineEdit &w);

            protected:
                Graphic::String     *_font;                     ///< the text in the text box.
                bool                _editable;                  ///< if true, the linedit will receive events from the keyboard.
                Graphic::String     *_fontUnderscore;           ///< store the undescore draw next to the editable text.
                bool                _fontUnderscoreDisplayed;   ///< if true, the underscore is displayed.
                Utils::Clock        _clock;                     ///< clock used to draw the underscore next to the editable text and make it blink.
        };
    }
}

#endif
