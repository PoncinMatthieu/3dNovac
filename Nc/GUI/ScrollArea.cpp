
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

    File Created At:        08/11/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "ScrollArea.h"

using namespace Nc;
using namespace Nc::GUI;

ScrollArea::ScrollArea(Corner x, Corner y, const Vector2i &pos, const Vector2i &size)
    : Widget(x, y, pos, size), _view(NULL)
{
    _scrollBarH = new ScrollBar(Left, Bottom, Vector2i(0, 0), size[0], Horizontal);
    _scrollBarV = new ScrollBar(Right, Top, Vector2i(0, 0), size[1], Vertical);
    AddComposedWidget(_scrollBarH);
    AddComposedWidget(_scrollBarV);
    _useStencil = true;
}

ScrollArea::ScrollArea(const ScrollArea &sc)
{
    _scrollBarH = NULL;
    _scrollBarV = NULL;
    Copy(sc);
}

ScrollArea &ScrollArea::operator = (const ScrollArea &sc)
{
    Widget::operator = (sc);
    Copy(sc);
    return *this;
}

void ScrollArea::Copy(const ScrollArea &sc)
{
    if (_scrollBarH != NULL)
    {
        RemoveComposedWidget(_scrollBarH);
        delete _scrollBarH;
    }
    if (_scrollBarV != NULL)
    {
        RemoveComposedWidget(_scrollBarV);
        delete _scrollBarV;
    }

    _view = NULL;
    _scrollBarH = (ScrollBar*)sc._scrollBarH->Clone();
    _scrollBarV = (ScrollBar*)sc._scrollBarV->Clone();
    AddComposedWidget(_scrollBarH);
    AddComposedWidget(_scrollBarV);
}

ScrollArea::~ScrollArea()
{
    delete _scrollBarH;
    delete _scrollBarV;
}

void ScrollArea::SetView(Widget *view)
{
    _view = view;

    Vector2i size;
    _view->GetReelSize(size);

    _scrollBarH->TotalSize(size[0]);
    _scrollBarH->Position(0);
    _scrollBarV->TotalSize(size[1]);
    _scrollBarV->Position(0);
}

void ScrollArea::Update()
{
    Widget::Update();

    Vector2i size;
    GetReelSize(size);

    if (_view != NULL)
    {
        Vector2i pos, size;
        _view->GetReelPos(pos);
        _view->GetReelSize(size);

        // if both scrollbar as to be rendered, reduce the size of the scrollbars
        if (size[0] > _size[0] && size[1] > _size[1])
        {
            _scrollBarH->Size(Vector2i(_size[0] - _scrollBarV->Size()[0], _scrollBarH->Size()[1]));
            _scrollBarV->Size(Vector2i(_scrollBarV->Size()[0], _size[1] - _scrollBarH->Size()[1]));
        }
    }

    _scrollBarH->PageSize(size[0]);
    _scrollBarV->PageSize(size[1]);
}

void ScrollArea::Draw(Graphic::SceneGraph *scene)
{
    if (_view != NULL)
    {
        Vector2i v = Vector2i(-_scrollBarH->Position(), -_scrollBarV->Position());
        if (v != _view->Pos())
            _view->Pos(v);
    }


    Widget::Draw(scene);
}

void ScrollArea::RenderBegin(Graphic::SceneGraph *scene)
{
    Widget::RenderBegin(scene);
}

void ScrollArea::RenderEnd(Graphic::SceneGraph *scene)
{
    if (_view != NULL)
    {
        Vector2i pos, size;
        _view->GetReelPos(pos);
        _view->GetReelSize(size);

        if (size[0] > _size[0])
            _scrollBarH->RenderNode(scene);
        if (size[1] > _size[1])
            _scrollBarV->RenderNode(scene);
    }

    Widget::RenderEnd(scene);
}
