
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

    File Created At:        04/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_BUTTON_H_
#define NC_GUI_BUTTON_H_

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a button
        /** Display and manage a button. */
        class LGUI  Button : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Button);

            public:
                Button(const std::string &text, Corner x = Left, Corner y = Top, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(0, 0),
                       const std::string &ttf = "arial", const std::string &lookName = "");
                Button(const std::string &text, Corner x, Corner y, const Vector2i &pos, int fontSize, int marginX, int marginY,
                       const std::string &ttf = "arial", const std::string &lookName = "");
                Button(const Button &w);
                Button &operator = (const Button &w);
                virtual ~Button();

                virtual ISceneNode  *Clone() const                  {return new Button(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Set the text of the widget */
                inline void         Text(const std::string &t)  {_font->Text(t);}

            protected:
                /** Update the geometry of the button */
                virtual void        Update();
                /** Render the button */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** The mouse button handler */
                virtual void		MouseButtonEvent(const System::Event &event);
                /** Send an event to the good engine */
                virtual void        execHanle();

            private:
                /** Initialise the widget */
                void                Initialize(const std::string &text, const Vector2i &size, const std::string &ttf, const std::string &lookName);
                /** Copy the widget */
                void                Copy(const Button &w);

            private:
                Graphic::Sprite     *_sprite;               ///< the sprite of the button
                Graphic::String     *_font;                 ///< The text of the button
                bool                _buttonPressed;         ///< Mark if the button is pressed
                Color               _colorDisable;          ///< The color of the disabled button
        };
    }
}

#endif

