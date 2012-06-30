
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

    File Created At:        17/09/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_VISITOR_VISITORS_H_
#define NC_GUI_VISITOR_VISITORS_H_

#include "../Define.h"
#include "WidgetVisitor.h"
#include "../SceneGraph.h"

namespace Nc
{
    namespace GUI
    {
        namespace Visitor
        {
            struct LGUI ResizedAll : public WidgetVisitor<ResizedAll>
            {
                ResizedAll();
                void VisitNode(Widget &w);
            };

            struct LGUI ReposedAll : public WidgetVisitor<ReposedAll>
            {
                ReposedAll();
                void VisitNode(Widget &w);
            };

            struct LGUI IsInhibited : public WidgetVisitor<IsInhibited, true>
            {
                IsInhibited();
                void VisitNode(const Widget &w);

                bool    result;
            };

            struct LGUI CheckFocus : public WidgetVisitor<CheckFocus>
            {
                CheckFocus(const Nc::System::Event &e, const Vector2i &mouseP);
                void VisitNode(Widget &w);

                const Nc::System::Event     &event;
                const Vector2i              &mousePos;
                Widget                      *childFocused;
            };


            /// Visitor allowing to retreive either the widget parent of the given widget or to retreive the scene graph if no widget parent has been found
            struct LGUI GetParentWidget : public WidgetVisitor<GetParentWidget, true>
            {
                GetParentWidget(const Widget *w);
                void VisitNode(const Widget &w);
                void VisitNode(const SceneGraph &s);

                const Widget            *widget;
                const Widget            *parent;
                const SceneGraph        *parentSceneGraph;
            };

            struct LGUI ChangeStates : public WidgetVisitor<ChangeStates>
            {
                ChangeStates();
                void VisitNode(Widget &w);
            };
        }
    }
}

#endif
