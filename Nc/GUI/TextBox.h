
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
/*-----------------------------------------------------------------------------


                    Implementation de la classe "gTextBox"
        permet l'affichage d'une text box, possibilite d'ecrire dedans

Herite de gWidgetLabeled

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_TEXTBOX_H_
#define NC_GUI_TEXTBOX_H_

#include "WidgetLabeled.h"

namespace Nc
{
    namespace GUI
    {
        /// to manage a text box
        class LGUI  TextBox : public WidgetLabeled
        {
            public:
                TextBox(const std::string &label, const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10),
                        Corner x = Left, Corner y = Top, Widget *parent = NULL, const std::string &ttf = "arial");
                ~TextBox();

                TextBox(const TextBox &w);
                TextBox &operator = (const TextBox &w);
                virtual void    Copy(const TextBox &w);
                virtual Widget* Clone() const               {return new TextBox(*this);}

                /** Set the text of the text box */
                void                                Text(const Utils::Unicode::UTF32 &t)        {_font->Text(t);}
                /** Return the text of the text box */
                inline const Utils::Unicode::UTF32  &Text() const                               {return _font->Text();}

            protected:
                /** update the geometry of the text box */
                virtual void Update();
                /** Render the text box */
                virtual void Draw(Graphic::ISceneGraph *scene);
                /** Fill the string of the text box */
                void GetData(std::string &data);

                /** the keyboard handler */
                virtual void KeyboardEvent(const System::Event &event);

                Graphic::String     *_font;         ///< the text in the text box

            private:
                Graphic::Drawable<Graphic::BasicVertexType::Colored2d, false>   _drawable;      ///< the drawable of the text box
        };
    }
}

#endif
