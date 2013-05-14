
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

    File Created At:        08/01/2013
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_GUI_WIDGET_TREEWIDGET_H_
#define NC_GUI_WIDGET_TREEWIDGET_H_

#include "ScrollArea.h"
#include "TreeItem.h"

namespace Nc
{
    namespace GUI
    {
        class LIB_NC_GUI TreeWidget : public ScrollArea
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT_VISITABLE(ScrollArea, System::Object, Nc::GUI::TreeWidget);

                struct TreeView : public Widget
                {
                    NC_SYSTEM_DEFINE_OBJECT_VISITABLE(Widget, System::Object, Nc::GUI::TreeWidget::TreeView);

                    TreeView(TreeWidget *widget, const AlignmentMask &alignment, const Vector2i &size);
                    TreeView(const TreeView &tv);
                    TreeView &operator = (const TreeView &tv);
                    ~TreeView();

                    /** Check item selection. */
                    virtual void            CheckItemFocus(const System::Event &event);

                    /** Resize the widget. Called when the widget or a parent has been resized. */
                    virtual void            Resize();

                    TreeWidget              *_widget;
                };

            public:
                TreeWidget(const AlignmentMask &alignment, const Vector2i &size);
                TreeWidget(const TreeWidget &tw);
                TreeWidget &operator = (const TreeWidget &tw);
                ~TreeWidget();

                /** The mouse button handler to choose an item. */
                void    MouseButtonEvent(const System::Event &event);

                /** Insert an item node into the tree. */
                void    Insert(Item *items);

            protected:
                /**
                    Method called just after the RenderChilds method.
                    Do nothing but can be redefined.
                */
                virtual void    RenderChildsEnd(Graphic::SceneGraph *scene);

            private:
                /** Copy the widget attributes. */
                void    Copy(const TreeWidget &tw);

            private:
                TreeView        *_treeView;
                Item            *_selectedItem;
        };
    }
}

#endif
