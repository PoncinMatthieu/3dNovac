
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

#ifndef NC_GUI_WIDGET_SUBWINDOW_H_
#define NC_GUI_WIDGET_SUBWINDOW_H_

#include <Nc/Graphics/Window/SubWindow.h>
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        /// Defines a subWindow Widget creating and managing a Graphic::SubWindow.
        /**
            Use a FrameBuffer to render the subwindow to render it into the widget.
        */
        class LGUI SubWindow : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_INVOKABLE(Widget, System::Object, System::Object, Nc::GUI::SubWindow);

            public:
                SubWindow(Graphic::IWindow *windowParent, const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0));
                SubWindow(const SubWindow &w);
                SubWindow &operator = (const SubWindow &w);
                virtual ~SubWindow();

                /** Resize the widget with the SubWindow. */
                virtual void            Resize();
                /** Repos the widget with the SubWindow. */
                virtual void            Reposed();

                /** \return the SubWindow. */
                Graphic::SubWindow      *GetSubWindow() const       {return _subWindow;}
                /** Enable the sub window for rendering. */
                void			        EnableSubWindow()			{_subWindow->Enable(true);}

            protected:
                /** Draw the result of the render off screen from the sub window. */
                virtual void            Draw(Graphic::SceneGraph *scene);

            protected:
                Graphic::SubWindow      *_subWindow;    ///< SubWindow used to simulate a virtual window.
                Graphic::Sprite         *_sprite;       ///< sprite used to render the result off screen of the sub window.
        };
    }
}

#endif
