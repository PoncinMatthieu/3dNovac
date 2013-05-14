
#include "../Visitor/Visitors.h"
#include "TreeItem.h"
#include "TreeWidget.h"

using namespace Nc;
using namespace Nc::GUI;

TreeItem::TreeItem(const Utils::Unicode::UTF32 &label, float fontSize, const Color &fontColor, const std::string &fontName, Nc::Graphic::PlainTextFormater::Style s)
    :   Item(label, fontSize, fontColor, fontName, s)
{
    _folded = false;
    _alwaysTestChildFocus = true;
}

TreeItem::~TreeItem()
{

}

void    TreeItem::Selected(bool st)
{
    if (_selected != st)
    {
        _selected = st;

        // get the parent tree view to notify the selection
        TreeWidget  *parentTreeWidget = GetParentTreeWidget();
        if (parentTreeWidget != NULL)
            parentTreeWidget->ItemChanged(this);

        // send a select event
        if (_selected)
            SendEvent(Event::ItemSelected);

        _stateChanged = true;
    }
}

void    TreeItem::Folded(bool st)
{
    if (_folded != st)
    {
        _folded = st;
        _alwaysTestChildFocus = !_folded;
        _stateChanged = true;
        Resized();
    }
}

void    TreeItem::PosChild(const Widget *w, Vector2i &pos) const
{
    Widget::PosChild(w, pos);
    pos[0] += 10;
}

Vector2i    TreeItem::ComputeSizePosRecursif(unsigned int depth)
{
    unsigned int    inter = 10;
    Vector2f        totalSize = _size;

    totalSize[0] += depth * inter;
    if (!_folded)
    {
        for (NodePolitic::ContainerType::iterator it = _childs.begin(); it != _childs.end(); ++it)
        {
            TreeItem *item = dynamic_cast<TreeItem*>(*it);
            if (item != NULL)
            {
                item->Pos(Vector2i(0, (totalSize[1] - PaddingTop())));
                Vector2i size = item->ComputeSizePosRecursif(depth + 1);
                if (totalSize[0] < size[0])
                    totalSize[0] = size[0];
                totalSize[1] += size[1];
            }
        }
    }
    return totalSize;
}

bool    TreeItem::RenderChildsBegin(Graphic::SceneGraph *scene)
{
    if (_folded)
        return false;
    return Item::RenderChildsBegin(scene);
}

void    TreeItem::DoubleClicked()
{
    Folded(!Folded());
}

TreeWidget  *TreeItem::GetParentTreeWidget()
{
    TreeWidget  *parentTreeWidget = NULL;
    Widget      *parent = this;
    while (parent != NULL && parentTreeWidget == NULL)
    {
        Visitor::GetParentWidget v(parent);
        v(*parent);
        parent = const_cast<Widget*>(v.parent);

        parentTreeWidget = dynamic_cast<TreeWidget*>(parent);
    }
    return parentTreeWidget;
}
