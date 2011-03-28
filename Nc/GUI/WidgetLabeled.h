
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


                    Implementation de la classe "gWidgetLabeled"
    Il s'agit d'un widget disposant d'un label, le label est draw a gauche du
                                    widget

Herite de gWidget

-----------------------------------------------------------------------------*/

#ifndef NOVAC_GRAPHIC_GUI_WIDGETLABELED_H_
#define NOVAC_GRAPHIC_GUI_WIDGETLABELED_H_

#include <string.h>

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  WidgetLabeled : public Widget
        {
            public:
                WidgetLabeled(const Utils::Unicode::UTF32 &label, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(10, 10), Corner x = Left, Corner y = Top,
                              Widget *parent = NULL, const std::string &ttf = "arial", Graphic::String::Style s = Graphic::String::Regular);
                virtual ~WidgetLabeled();

                WidgetLabeled(const WidgetLabeled &w);
                WidgetLabeled &operator = (const WidgetLabeled &w);
                virtual void    Copy(const WidgetLabeled &w);
                virtual Widget* Clone() const               {return new WidgetLabeled(*this);}

                virtual void            Size(const Vector2f &size);
                virtual Vector2f        GetReelSize() const;

                virtual void                    CreateLabel(const Utils::Unicode::UTF32 &l, const std::string &ttf, Graphic::String::Style s);
                inline const Vector2f           &LabelSize()                        {return _label->Size();}
                inline void                     Label(const Utils::Unicode::UTF32 &text)      {_label->Text(text); _stateChange = true;}
                inline void                     LabelColor(const Color &color)  {_label->SetColor(color); _stateChange = true;}
                inline const Graphic::String    *GetLabel() const          {return _label;}

            protected:
                virtual void    Draw(Graphic::ISceneGraph *scene);

            protected:
                Graphic::String *_label;
        };
    }
}

#endif
