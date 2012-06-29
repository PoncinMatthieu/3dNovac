
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

    File Created At:        08/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_SCROLLAREA_H_
#define NC_GUI_SCROLLAREA_H_

#include "Widget.h"
#include "ScrollBar.h"

namespace Nc
{
    namespace GUI
    {
        /// Provide a scrolling view onto another widget
        /**
            Use the size of the first widget child to render the scroll area.
        */
        class LGUI ScrollArea : public Widget
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::ScrollArea);

            public:
                ScrollArea(const AlignmentMask &alignment = Left | Top, const Vector2i &size = Vector2i(0, 0) );
                ScrollArea(const ScrollArea &sc);
                ScrollArea &operator = (const ScrollArea &sc);
                virtual ~ScrollArea();

                virtual ISceneNode  *Clone() const          {return new ScrollArea(*this);}

                /** Set the widget which will be used to setup the scroll area by using the size of the view object */
                void                SetView(Widget *view);

            protected:
                /** Update the geometry of the scroll area */
                virtual void        Update();
                /** Check the state and update the matrix before rendering */
                virtual void        RenderBegin(Graphic::SceneGraph *scene);
                /** draw Debug GUI and update the matrix before rendering */
                virtual void        RenderEnd(Graphic::SceneGraph *scene);
                /** Render the scroll area */
                virtual void        Draw(Graphic::SceneGraph *scene);

            private:
                void                Copy(const ScrollArea &sc);

            protected:
                Widget              *_view;             ///< view widget used to setup the scroll area
                ScrollBar           *_scrollBarH;
                ScrollBar           *_scrollBarV;
        };
    }
}

#endif
