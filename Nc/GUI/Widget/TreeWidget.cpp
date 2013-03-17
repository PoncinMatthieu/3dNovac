
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
    : ScrollArea(alignment, size)
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
    _treeView = static_cast<TreeView*>(tw._treeView->Clone());
}

void    TreeWidget::Insert(Item *items)
{
    _treeView->AddChild(items);
    _treeView->Resized();
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

struct ComputeTotalViewSizeVisitor : public GUI::Visitor::WidgetVisitor<ComputeTotalViewSizeVisitor>
{
    ComputeTotalViewSizeVisitor(TreeWidget::TreeView *tv)
        : GUI::Visitor::WidgetVisitor<ComputeTotalViewSizeVisitor>(  Utils::Metaprog::Seq<Item>::Type(),
                                                                Utils::Metaprog::Seq<Widget>::Type(), Graph::VisitChilds, true, true),
            treeView(tv)
    {
    }

    void VisitNode(Widget &w)
    {
    }

    void VisitNode(Item &n)
    {
        Vector2f    size = n.Text()->Size();
        int         depth = (n.Depth() - 1) * 10;

        size[0] += depth;

        if (totalSize[0] < size[0])
            totalSize[0] = size[0];
        totalSize[1] += size[1];

        n.Pos(Vector2i(depth, -(totalSize[1])));
        totalSize[1] += 5;
    }

    Vector2i                totalSize;
    TreeWidget::TreeView    *treeView;
};

void    TreeWidget::TreeView::Resize()
{
    CALLSTACK_INFO("TreeWidget::TreeView::Resize()")
    Widget::Resize();

    bool resized = false;

    // compute the size
    ComputeTotalViewSizeVisitor v(this);
    v(*this);

    if (_size[0] != v.totalSize[0] + PaddingH())
    {
        _size[0] = v.totalSize[0] + PaddingH();
        resized = true;
    }
    if (_size[1] != v.totalSize[1] + PaddingV())
    {
        _size[1] = v.totalSize[1] + PaddingV();
        resized = true;
    }

    if (resized)
        _widget->Resize();
}

void    TreeWidget::TreeView::UpdateState()
{
    Widget::UpdateState();
}

// visitor used to go through every item and render them
struct DrawItemsVisitor : public GUI::Visitor::WidgetVisitor<DrawItemsVisitor>
{
    DrawItemsVisitor(Graphic::SceneGraph *s)
        :   GUI::Visitor::WidgetVisitor<DrawItemsVisitor>( Utils::Metaprog::Seq<Item>::Type(),
                                                           Utils::Metaprog::Seq<Widget>::Type(),
                                                           Graph::VisitChilds, true, true),
            scene(s)
    {
    }

    void VisitNode(Widget &w)
    {
    }

    void VisitNode(Item &n)
    {
        n.RenderNode(scene);
    }

    Graphic::SceneGraph *scene;
};

void    TreeWidget::TreeView::Draw(Graphic::SceneGraph *scene)
{
    Widget::Draw(scene);

    Vector2i pos;
    pos.data[0] = PaddingLeft();
    pos.data[1] = _size.data[1] - PaddingTop();

    scene->PushModelMatrix();
    scene->ModelMatrix().AddTranslation(pos.data[0], pos.data[1], 0.f); // translation sur la position relative au Corner

    DrawItemsVisitor v(scene);
    v(*this);

    scene->PopModelMatrix();
}


