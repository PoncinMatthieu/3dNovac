
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

#ifndef NC_GUI_WIDGETLABELED_H_
#define NC_GUI_WIDGETLABELED_H_

#include <string.h>

#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// To manage a widget with a label
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

                /** Set the size if the widget */
                virtual void            Size(const Vector2f &size);
                /** Return the reel size of the widget (including the label) */
                virtual Vector2f        GetReelSize() const;

                /** Create the label with a text, a font and a style */
                virtual void                    CreateLabel(const Utils::Unicode::UTF32 &l, const std::string &ttf, Graphic::String::Style s);
                /** Return the label size */
                inline const Vector2f           &LabelSize()                        {return _label->Size();}
                /** Set the label text */
                inline void                     Label(const Utils::Unicode::UTF32 &text)      {_label->Text(text); _stateChange = true;}
                /** Set the label color */
                inline void                     LabelColor(const Color &color)  {_label->SetColor(color); _stateChange = true;}
                /** Return the label */
                inline const Graphic::String    *GetLabel() const          {return _label;}

            protected:
                /** Render the widget labeled */
                virtual void    Draw(Graphic::ISceneGraph *scene);

            protected:
                Graphic::String *_label;        ///< the label (string)
        };
    }
}

#endif
