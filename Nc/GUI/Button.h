
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
        class LGUI  Button : public Widget
        {
            public:
                Button(const std::string &text, const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10), Corner x = Left, Corner y = Top,
                        Widget *parent = NULL, const std::string &ttf = "arial", const Utils::FileName& texture = Utils::FileName("Nc:GUI:button.png"));
                virtual ~Button();

                Button(const Button &w);
                Button &operator = (const Button &w);
                virtual void    Copy(const Button &w);
                virtual Widget* Clone() const               {return new Button(*this);}

                /** Set the text of the widget */
                inline void     Text(const std::string &t)  {_font->Text(t);}


            protected:
                /** Update the geometry of the button */
                virtual void Update();
                /** Render the button */
                virtual void Draw(Graphic::ISceneGraph *scene);

                /** The mouse button handler */
                void MouseButtonEvent(const System::Event &event);
                /** Send an event to the good engine */
                virtual void execHanle();

            private:
                Graphic::Material<Graphic::BasicVertexType::Textured2d>             *_material;     ///< the material used to render the button
                Graphic::Drawable<Graphic::BasicVertexType::Textured2d, false>      _drawable;      ///< the drawable used to render the button

                Graphic::String         *_font;                 ///< The text of the button
                int                     _textWidth;             ///< The width of the text
                int                     _textHeight;            ///< The height of the text
                bool                    _buttonPressed;         ///< Mark if the button is pressed
                Color                   _colorDisable;          ///< The color of the disabled button
        };
    }
}

#endif

