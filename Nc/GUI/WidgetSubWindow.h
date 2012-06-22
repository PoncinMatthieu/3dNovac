
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

    File Created At:        22/06/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGETSUBWINDOW_H_
#define NC_GUI_WIDGETSUBWINDOW_H_

#include <Nc/Graphics/Window/Window/SubWindow.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Widget creating and managing a Graphic::SubWindow
        /**
            Use a FrameBuffer to render the subwindow to render it into the widget.
        */
        class WidgetSubWindow : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(Widget, System::Object, System::Object, Nc::GUI::WidgetSubWindow);

            public:
                WidgetSubWindow(Graphic::Window *windowParent, Corner x = Left, Corner y = Top, const Vector2i &pos = Vector2i(0, 0), const Vector2i &size = Vector2i(0, 0));
                WidgetSubWindow(const WidgetSubWindow &w);
                WidgetSubWindow &operator = (const WidgetSubWindow &w);
                virtual ~WidgetSubWindow();

                /** Resize the widget with the SubWindow */
                virtual void            Resized();

                /** \return the SubWindow */
                Graphic::SubWindow      *SubWindow() const          {return _subWindow;}

            protected:
                Graphic::SubWindow                  *_subWindow;    ///< SubWindow used to simulate a virtual window
//                Nc::Graphic::GL::FrameBuffer		_fboPass1;
        };
    }
}

#endif
