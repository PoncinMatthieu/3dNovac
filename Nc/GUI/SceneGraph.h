
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
#include "Define.h"
#include "Widget.h"

namespace Nc
{
    /// This namespace provide many Widget and a SceneGraph to create and render a 2d Graphical User Interface
    namespace GUI
    {
        /// Manage a 2d scene graph with the management the interaction of widgets
        class LGUI  SceneGraph : public Graphic::I2dSceneGraph
        {
            public:
                SceneGraph(Graphic::Camera2d *camera);
                virtual ~SceneGraph();

                /** Render the widgets */
                virtual void RenderIn2D();

                // accessors
                /** Return the list of widgets */
                inline const ListPWidget                &Widgets() const        {return _listWidget;}
                /** Return the list of object */
                inline const Graphic::ListPObject       &Objects() const        {return _listObject;}

                /** Add an object to the scene */
                void AddObject(Graphic::Object *object);
                /** Add a widget to the scene */
                void AddWidget(Widget *widget);
                /** Bring to front the given widget */
                void BringToFront(Widget *widget);
                /** Remove the given object from the list, if del is true then the delete it */
                void DeleteObject(Graphic::Object *object, bool del = true);
                /** clear the list of object and widget, if del is true then the delete it */
                void Clear(bool del);

                /** Return true if a widget is focused */
                bool Focused()              {return (_widgetFocused != NULL);}
                /** Unfocus the current focuse widget */
                void Unfocus()              {if (_widgetFocused != NULL) _widgetFocused->Focus(false); _widgetFocused = NULL;}
                /** Unfocus the given widget */
                void Unfocus(Widget *w)   {w->Focus(false); _widgetFocused = NULL;}
                /** Focus the given widget */
                void Focus(Widget *w)    {_widgetFocused = w; _widgetFocused->Focus(true);}

                /** Manage the window events by dispatching the event to the good widget */
                void ManageWindowEvent(const Nc::System::Event &event);

            private:
                ListPWidget             _listWidget;        ///< the list of widget
                Graphic::ListPObject    _listObject;        ///< the list of object
                Widget                  *_widgetFocused;    ///< the current focused widget
                System::Mutex           _mutex;             ///< the mutex that protect the lists
        };
    }
}

#endif
