
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

    File Created At:        02/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_GUISCENEGRAPH_H_
#define NC_GUI_GUISCENEGRAPH_H_

#include <list>
#include <Nc/Graphics/Camera/Camera2d.h>
#include "Define.h"
#include "Widget.h"

namespace Nc
{
    /// This namespace provide many Widget and a SceneGraph to create and render a 2d Graphical User Interface
    namespace GUI
    {
        /// Manage a 2d scene graph with the management of the interaction of widgets and reception of events
        class LGUI  SceneGraph : public Graphic::SceneGraph
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Graphic::SceneGraph, Nc::System::Object, Nc::GUI::SceneGraph);

            public:
                SceneGraph();
                SceneGraph(const SceneGraph &scene);
                SceneGraph &operator = (const SceneGraph &scene);
                virtual ~SceneGraph();

                virtual ISceneNode  *Clone() const      {return new SceneGraph(*this);}

                /** Return true if a widget is focused */
                bool Focused()                          {return (_widgetFocused != NULL);}
                /** Unfocus the current focuse widget */
                void Unfocus()                          {if (_widgetFocused != NULL) _widgetFocused->Focus(false); _widgetFocused = NULL;}
                /** Unfocus the given widget */
                void Unfocus(Widget *w)                 {w->Focus(false); _widgetFocused = NULL;}
                /** Focus the given widget */
                void Focus(Widget *w)                   {_widgetFocused = w; _widgetFocused->Focus(true);}

                /** Manage the window events by dispatching the event to the good widget */
                void ManageWindowEvent(const Nc::System::Event &event);

                /** Bring to front the given widget */
                void BringToFront(Widget *w);

                /** Remove the given widget */
                void RemoveWidget(Widget *w);

            private:
                Widget                  *_widgetFocused;    ///< the current focused widget
        };
    }
}

#endif
