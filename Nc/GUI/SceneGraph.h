
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
/*-----------------------------------------------------------------------------

                    Implementation de la classe "gGUI"

                Gestion complete et affichage d'une GUI in game

Herite de Object2d

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_GUISCENEGRAPH_H_
#define NC_GUI_GUISCENEGRAPH_H_

#include <list>
#include "Define.h"
#include "Widget.h"

namespace Nc
{
    namespace GUI
    {
        class LGUI  SceneGraph : public Graphic::I2dSceneGraph
        {
            public:
                SceneGraph(Graphic::Camera2d *camera);
                virtual ~SceneGraph();

                virtual void RenderIn2D();

                // accessors
                inline const ListPWidget                &Widgets() const        {return _listWidget;}
                inline const Graphic::ListPObject       &Objects() const        {return _listObject;}

                void AddObject(Graphic::Object *object);
                void AddWidget(Widget *widget);
                void BringToFront(Widget *widget);
                void DeleteObject(Graphic::Object *object, bool del = true);
                void Clear(bool del);

                bool Focused()              {return (_widgetFocused != NULL);}
                void Unfocus()              {if (_widgetFocused != NULL) _widgetFocused->Focus(false); _widgetFocused = NULL;}
                void SetFocus(Widget *w)    {_widgetFocused = w; _widgetFocused->Focus(true);}
                void LostFocus(Widget *w)   {w->Focus(false); _widgetFocused = NULL;}

                void ManageWindowEvent(const Nc::System::Event &event);

            private:
                ListPWidget             _listWidget;
                Graphic::ListPObject    _listObject;
                Widget                  *_widgetFocused;
                System::Mutex           _mutex;
        };
    }
}

#endif
