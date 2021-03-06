
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

#include "TreeWidget.h"
#include "../Visitor/WidgetVisitor.h"
#include "../Look/Looks.h"

using namespace Nc;
using namespace Nc::GUI;


TreeWidget::TreeWidget(const AlignmentMask &alignment, const Vector2i &size)
    : ScrollArea(alignment, size), _selectedItem(NULL)
{
    _treeView = new TreeView(this, Left | Top, size);
    _treeView->Padding(BoxEdges(5,5,5,5));
    _treeView->Percent(Vector2f(100, 0));
    AddComposedWidget(_treeView);
    SetView(_treeView);
}

TreeWidget::TreeWidget(const TreeWidget &tw)
    : ScrollArea(tw)
{
    Copy(tw);
}

TreeWidget &TreeWidget::operator = (const TreeWidget &tw)
{
    ScrollArea::operator = (tw);
    delete _treeView;
    Copy(tw);
    return *this;
}

TreeWidget::~TreeWidget()
{
    delete _treeView;
}

void    TreeWidget::Copy(const TreeWidget &tw)
{
    _selectedItem = NULL;
    _treeView = static_cast<TreeView*>(tw._treeView->Clone());
}

void    TreeWidget::Insert(TreeItem *item)
{
    if (item->Selected())
        _selectedItem = item;
    _treeView->AddChild(item);
    _treeView->Resized();
}

void    TreeWidget::ItemChanged(TreeItem *item)
{
    if (_selectedItem != item)
    {
        _selectedItem->Unselect();
        _selectedItem = item;
    }
}

void    TreeWidget::RenderChildsEnd(Graphic::SceneGraph *scene)
{
    _treeView->RenderNode(scene);
    ScrollArea::RenderChildsEnd(scene);
}

TreeWidget::TreeView::TreeView(TreeWidget *widget, const AlignmentMask &alignment, const Vector2i &size)
    : Widget(alignment, size), _widget(widget)
{
    Resize();
}

TreeWidget::TreeView::TreeView(const TreeView &tv)
    : Widget(tv), _widget(tv._widget)
{
    Resize();
}

TreeWidget::TreeView &TreeWidget::TreeView::operator = (const TreeView &tv)
{
    Widget::operator = (tv);
    _widget = tv._widget;
    Resize();
    return *this;
}

TreeWidget::TreeView::~TreeView()
{
}

void    TreeWidget::TreeView::Resize()
{
    CALLSTACK_INFO()
    Widget::Resize();

    bool resized = false;

    // compute total size and positions of every item using a recursive method
    Vector2i    totalSize;
    for (NodePolitic::ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
    {
        TreeItem *item = dynamic_cast<TreeItem*>(*it);
        if (item != NULL)
        {
            item->Pos(Vector2i(0, totalSize[1]));
            Vector2i size = item->ComputeSizePosRecursif(0);
            if (totalSize[0] < size[0])
                totalSize[0] = size[0];
            totalSize[1] += size[1];
        }
    }

    if (_size[0] != totalSize[0] + PaddingH())
    {
        _size[0] = totalSize[0] + PaddingH();
        resized = true;
    }
    if (_size[1] != totalSize[1] + PaddingV())
    {
        _size[1] = totalSize[1] + PaddingV();
        resized = true;
    }

    if (resized)
        _widget->Resize();
}

