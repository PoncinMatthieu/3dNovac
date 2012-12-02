
/*-----------------------------------------------------------------------------

	3dNovac Graphics
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

    File Created At:        10/03/2012
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_VISITOR_WIDGETVISITOR_H_
#define NC_GUI_VISITOR_WIDGETVISITOR_H_

#include <Nc/Graphics/Scene/SceneNodeVisitor.h>
#include "../Define.h"
#include "../Widget/Widget.h"

namespace Nc
{
    namespace GUI
    {
        namespace Visitor
        {
            /// Base class used to define a widget visitor.
            template<typename VisitorType, bool IsConst, typename ReturnType>
            class WidgetVisitor : public Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>
            {
                public:
                    template<typename ToVisitList, typename VisitableList>
                    WidgetVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, Graph::VisitTarget visitTarget = Graph::VisitChilds, bool postVisits = true, bool direction = true)
                        : Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>(toVisitList, visitableList, visitTarget, postVisits, direction)
                    {}

                    template<typename T>
                    void InvokeChilds(T &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>::InvokeChilds(n);
                    }

                    void InvokeChilds(Widget &n)
                    {
                        for (ListPWidget::iterator it = n._composedWidget.begin(); it != n._composedWidget.end(); ++it)
                            (*this)(**it);
                        Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>::InvokeChilds(n);
                    }

                    template<class T>
                    void InvokeParents(T &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>::InvokeParents(n);
                    }

                    void InvokeParents(Widget &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, IsConst, ReturnType>::InvokeParents(n);
                        if (n._owner != NULL)
                            (*this)(*n._owner);
                    }
            };

            /// Base class used to define a const widget visitor.
            template<typename VisitorType, typename ReturnType>
            class WidgetVisitor<VisitorType, true, ReturnType> : public Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>
            {
                public:
                    template<typename ToVisitList, typename VisitableList>
                    WidgetVisitor(const ToVisitList &toVisitList, const VisitableList &visitableList, Graph::VisitTarget visitTarget = Graph::VisitChilds, bool postVisits = true, bool direction = true)
                        : Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>(toVisitList, visitableList, visitTarget, postVisits, direction)
                    {}

                    template<typename T>
                    void InvokeChilds(const T &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>::InvokeChilds(n);
                    }

                    void InvokeChilds(const Widget &n)
                    {
                        for (ListPWidget::const_iterator it = n._composedWidget.begin(); it != n._composedWidget.end(); ++it)
                            (*this)(**it);
                        Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>::InvokeChilds(n);
                    }

                    template<class T>
                    void InvokeParents(const T &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>::InvokeParents(n);
                    }

                    void InvokeParents(const Widget &n)
                    {
                        Graphic::SceneNodeVisitor<VisitorType, true, ReturnType>::InvokeParents(n);
                        if (n._owner != NULL)
                            (*this)(*n._owner);
                    }
            };
        }
    }
}

#endif
