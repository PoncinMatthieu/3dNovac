
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
#include "../Visitor/Visitors.h"

using namespace Nc;
using namespace Nc::GUI;

ScrollArea::ScrollArea(const AlignmentMask &alignment, const Vector2i &size)
    : Widget(alignment, size), _view(NULL)
{
    _alwaysReceiveEvents = true; // to manage the scroll correctly we need to receive events even if we don't have the focus.
    _scrollBarH = new ScrollBar(Left | Bottom, Horizontal);
    _scrollBarH->Percent(Vector2f(100, 0));
    _scrollBarH->Pos(Vector2f(0, -_scrollBarH->Size()[1]));

    _scrollBarV = new ScrollBar(Right | Top, Vertical);
    _scrollBarV->Percent(Vector2f(0, 100));
    _scrollBarV->Pos(Vector2f(-_scrollBarV->Size()[0], 0));

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
    _scrollBarH = static_cast<ScrollBar*>(sc._scrollBarH->Clone());
    _scrollBarV = static_cast<ScrollBar*>(sc._scrollBarV->Clone());
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
    Resized();
}

void    ScrollArea::Update(float elapsedTime)
{
    Widget::Update(elapsedTime);


}

void ScrollArea::MouseButtonEvent(const Nc::System::Event &e)
{
    if (e.type == System::Event::MouseWheelMoved)
    {
        Vector2i mousePos = static_cast<Graphic::WindowInput*>(e.emitter)->MousePositionInGLCoord();
        if (Widget::TestFocus(*this, mousePos))
            _scrollBarV->MouseWheelEvent(e);
    }
}

void ScrollArea::Resize()
{
    Widget::Resize();

    if (_view != NULL)
    {
        Vector2i pageSize(_size);
        pageSize[0] -= PaddingH();
        pageSize[1] -= PaddingV();

        // disable the scrollbar which doesn't need to be rendered
        // with set a padding when a scroll bar is rendered
        bool _scrollBarHState = _scrollBarH->Enabled();
        bool _scrollBarVState = _scrollBarV->Enabled();
        if (_view->Size()[0] > pageSize[0])
        {
            _scrollBarH->Enable();
            PaddingBottom(_scrollBarH->Size()[1]);
        }
        else
        {
            _scrollBarH->Disable();
            PaddingBottom(0);
        }

        if (_view->Size()[1] > pageSize[1])
        {
            _scrollBarV->Enable();
            PaddingRight(_scrollBarV->Size()[0]);
        }
        else
        {
            _scrollBarV->Disable();
            PaddingRight(0);
        }

        // if the state of the scrollbars changed notify the changement to every child
        if (_scrollBarHState != _scrollBarH->Enabled() || _scrollBarVState != _scrollBarV->Enabled())
        {
            Visitor::ResizedAll resizedAll;
            resizedAll(*this);
        }

        if (_scrollBarH->Position() > _view->Size()[0])
            _scrollBarH->Position(_view->Size()[0]);
        else if (_scrollBarH->PositionChangedByUser() && _scrollBarH->Position() + _scrollBarH->PageSize() == _scrollBarH->TotalSize())
            _scrollBarH->Position(_view->Size()[0] - pageSize[0]);

        if (_scrollBarV->Position() > _view->Size()[1])
            _scrollBarV->Position(_view->Size()[1]);
        else if (_scrollBarV->PositionChangedByUser() && _scrollBarV->Position() + _scrollBarV->PageSize() == _scrollBarV->TotalSize())
            _scrollBarV->Position(_view->Size()[1] - pageSize[1]);

        _scrollBarH->TotalSize(_view->Size()[0]);
        _scrollBarV->TotalSize(_view->Size()[1]);

        _scrollBarH->PageSize(pageSize[0]);
        _scrollBarV->PageSize(pageSize[1]);
    }
}


void ScrollArea::Draw(Graphic::SceneGraph *scene)
{
    if (_view != NULL)
        _view->Pos(Vector2i(-_scrollBarH->Position(), -_scrollBarV->Position()));

    Widget::Draw(scene);
}

void ScrollArea::RenderBegin(Graphic::SceneGraph *scene)
{
    Widget::RenderBegin(scene);
}

void ScrollArea::RenderEnd(Graphic::SceneGraph *scene)
{
    _scrollBarH->RenderNode(scene);
    _scrollBarV->RenderNode(scene);

    Widget::RenderEnd(scene);
}
