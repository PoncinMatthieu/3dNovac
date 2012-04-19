
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

#ifndef NC_GUI_TEXTBOX_H_
#define NC_GUI_TEXTBOX_H_

#include "WidgetLabeled.h"

namespace Nc
{
    namespace GUI
    {
        /// to manage a text box
        class LGUI  LineEdit : public WidgetLabeled
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(WidgetLabeled, System::Object, Nc::GUI::LineEdit);

            public:
                LineEdit(const std::string &label, Corner x = Left, Corner y = Top, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(0, 0), const std::string &ttf = "arial", const std::string &looksName = "");
                LineEdit(const LineEdit &w);
                LineEdit &operator = (const LineEdit &w);
                ~LineEdit();

                virtual ISceneNode  *Clone() const              {return new LineEdit(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Set the text of the text box */
                void                                Text(const Utils::Unicode::UTF32 &t)        {_font->Text(t);}
                /** Return the text of the text box */
                inline const Utils::Unicode::UTF32  &Text() const                               {return _font->Text();}

            protected:
                /** update the geometry of the text box */
                virtual void Update();
                /** Render the text box */
                virtual void Draw(Graphic::SceneGraph *scene);
                /** Fill the string of the text box */
                void GetData(std::string &data);

                /** the keyboard handler */
                virtual void KeyboardEvent(const System::Event &event);

            private:
                /** Copy the widget */
                void        Copy(const LineEdit &w);

            protected:
                Graphic::String     *_font;         ///< the text in the text box

            private:
                unsigned int        _indexDrawable;
        };
    }
}

#endif
