
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

#ifndef NC_GUI_WIDGET_BUTTON_H_
#define NC_GUI_WIDGET_BUTTON_H_

#include "Widget.h"
#include "../Look/Looks.h"

namespace Nc
{
    namespace GUI
    {
        /// Define a button Widget.
        /**
            The button widget use allow to send an event to the specified engine, it only display a textual label.
            The button can be set as a checkable button.

            The button send a Toggle event when the button is toggled.
        */
        class LGUI  Button : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::Button);

            public:
                Button(const std::string &text, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0),
                       const std::string &ttf = "arial", const std::string &lookName = "");
                Button(const Button &w);
                Button &operator = (const Button &w);
                virtual ~Button();

                virtual ISceneNode  *Clone() const                  {return new Button(*this);}
                virtual void        ToString(std::ostream &os) const;

                /** Set the text of the widget. */
                inline void         PlainText(const Utils::Unicode::UTF32 &t)       {_font->PlainText(t);}

                /** Set whether the button is checkable or not. */
                inline void         Checkable(bool state)                           {_checkable = state; _stateChanged = true;}
                /** \return true if the button is checkable. */
                inline bool         Checkable() const                               {return _checkable;}

                /** Set the current status of the button. */
                inline void         Toggled(bool state)                             {_toggled = state; _stateChanged = true;}
                /** \return true if the button is currently toggled. */
                inline bool         Toggled() const                                 {return _toggled;}

            protected:
                /** Update the geometry of the button. */
                virtual void        UpdateState();
                /** Render the button. */
                virtual void        Draw(Graphic::SceneGraph *scene);

                /** The mouse button handler. */
                virtual void		MouseButtonEvent(const System::Event &event);

            private:
                /** Initialise the widget. */
                void                Initialize(const std::string &text, const Vector2i &size, const std::string &ttf, const std::string &lookName);
                /** Copy the button. */
                void                Copy(const Button &w);
                /** Delete the button. */
                void                DeleteButton();

            protected:
                StripLook               *_buttonLook;           ///< look used to render the button in it's normal state.
                StripLook               *_buttonLookToggled;    ///< look used to render the button pressed.
                Graphic::Text           *_font;                 ///< The text of the button.
                float                   _charSize;              ///< Recorde the charsize used for the font.
                bool                    _checkable;             ///< define if the button is a toggle button or not.
                bool                    _toggled;               ///< define if the button is toggled or not.
                Color                   _colorDisable;          ///< The color of the disabled button.
                System::Mouse::Button   _mouseButton;           ///< mouse button used to toggle the button. Default is Left.

            private:
                bool                    _buttonPressed;         ///< true if the button as just been pressed.
        };
    }
}

#endif

