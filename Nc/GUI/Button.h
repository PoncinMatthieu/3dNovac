
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
/*-----------------------------------------------------------------------------


                        Implementation de la classe "gButton"

            permet l'affichage d'un button, et la definision d'une action
                                sur l'appui de celui ci

Herite de gWidget

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_GUI_BUTTON_H_INCLUDED
#define NOVAC_GRAPHIC_GUI_BUTTON_H_INCLUDED

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  Button : public Widget
        {
            public:
                Button(const std::string &text, const Vector2f &pos = Vector2f(0, 0), const Vector2f &size = Vector2f(10, 10), Corner x = Left, Corner y = Top,
                        Widget *parent = NULL, const std::string &ttf = "arial", const Utils::FileName& texture = Utils::FileName("button.png"));
                virtual ~Button();

                inline void     Text(const std::string &t)  {_font->Text(t);}

                Button(const Button &w);
                Button &operator = (const Button &w);
                virtual void    Copy(const Button &w);
                virtual Widget* Clone() const               {return new Button(*this);}

            protected:
                virtual void Update();
                virtual void Draw(Graphic::ISceneGraph *scene);
                void MouseButtonEvent(const System::Event &event);
                virtual void execHanle();

            private:
                Graphic::Material<Graphic::BasicVertexType::Textured2d>                     *_material;
                Graphic::MaterialConfig<Graphic::BasicVertexType::Textured2d>               _config;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Textured2d, false>    _geometry;

                Graphic::String         *_font;
                int                     _textWidth;
                int                     _textHeight;
                bool                    _buttonPressed;
                Color                   _colorDisable;
        };
    }
}

#endif

