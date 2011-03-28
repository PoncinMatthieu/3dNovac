
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

    File Created At:        04/05/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------


                        Implementation de la classe "gWindowBox"

                permet l'affichage et la gestion de box sur le hud

Herite de Object2d

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GUI_WINDOWBOX_H_INCLUDED
#define NOVAC_GUI_WINDOWBOX_H_INCLUDED

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  WindowBox : public Widget
        {
            public:
                WindowBox(const std::string &title, const Vector2f &pos, const Vector2f &size, Corner x = Top, Corner y = Left, Widget *parent = NULL);
                virtual ~WindowBox();

                WindowBox(const WindowBox &w);
                WindowBox &operator = (const WindowBox &w);
                virtual void    Copy(const WindowBox &w);
                virtual Widget* Clone() const               {return new WindowBox(*this);}

                void ChangeTitle(const std::string &title, const std::string &ttf);

                inline int  TitleHeight() const                             {return _titleHeight;}
                inline void DrawTitleState(bool state)                      {_drawTitle = state; _stateChange = true;}
                inline void SetColor(const Color &cInside, const Color &cTitle1, const Color &cTitle2, const Color &cBorder)
                                                                            {_color = cInside; _titleColor1 = cTitle1; _titleColor2 = cTitle2; _edgeColor = cBorder; _stateChange = false;}
                inline void FillColor(const Color &color)               {_color = color; _stateChange = false;}

                virtual Vector2f    GetReelSize() const;

            protected:
                WindowBox(const std::string &title, const std::string &ttf = "arial"); // constructeur par defaut protected
                virtual void        MouseMotionEvent(const System::Event&) {}
                virtual void        MouseButtonEvent(const System::Event&) {}

                virtual void        Update();
                virtual void        Draw(Graphic::ISceneGraph *scene);
                virtual Vector2f    TranslateChild(const Corner corner[2]) const;

                Color               _color;
                bool                _dragable;

                Graphic::String     *_title;
                Color               _titleColor1;
                Color               _titleColor2;
                int                 _titleHeight;
                bool                _drawTitle;

            private:
                Graphic::MaterialConfig<Graphic::BasicVertexType::Colored2d>                _config;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Colored2d, false>     _geometryTitle;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Colored2d, false>     _geometryBox;
                Graphic::GL::GeometryBuffer<Graphic::BasicVertexType::Colored2d, false>     _geometryEdge;
        };
    }
}

#endif

