
#include "TreeItem.h"

using namespace Nc;
using namespace Nc::GUI;

TreeItem::TreeItem(const Utils::Unicode::UTF32 &label, float fontSize, const Color &fontColor, const std::string &fontName, Nc::Graphic::PlainTextFormater::Style s)
    :   Item(label, fontSize, fontColor, fontName, s),
        _folded(false)
{
}

TreeItem::~TreeItem()
{

}

void    TreeItem::Folded(bool st)
{
    _folded = st;
    _stateChanged = true;
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
    return totalSize;
}


