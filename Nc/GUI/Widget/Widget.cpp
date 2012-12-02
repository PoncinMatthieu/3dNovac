
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

    File Created At:        06/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include <Nc/Core/Engine/Manager.h>
#include <Nc/Graphics/Object/BasicMeshCreator.h>
#include "Widget.h"
#include "../Visitor/Visitors.h"
#include "../Look/Looks.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;

Widget::Widget(const AlignmentMask &alignment, const Vector2i &size)
    : Object(), Handler()
{
    Init(size, alignment);
}

void Widget::Init(const Vector2i &size, const AlignmentMask &alignment)
{
    _childFocused = NULL;
    _inhibit = false;
    _focus = false;
    _generateHandleAtEnterFocus = false;
    _resizable = true;
    _size = size;
    _alignment = alignment;
    _useStencil = false;
    _margin.Init(0, 0, 0, 0);
    _padding.Init(0, 0, 0, 0);
    _widgetLook = NULL;
    _owner = NULL;

    _reposed = true;
    _resized = true;
    _stateChanged = true;
}

Widget::Widget(const Widget &w)
    : Graphic::Object(w), Handler(w), _widgetLook(NULL)
{
    Copy(w);
}

Widget &Widget::operator = (const Widget &w)
{
    Graphic::Object::operator = (w);
    Handler::operator = (w);
    Copy(w);
    return *this;
}

Widget::~Widget()
{
    if (_widgetLook)
        delete _widgetLook;
}

void Widget::Copy(const Widget &w)
{
    _childFocused = NULL;
    _inhibit = w._inhibit;
    _resizable = w._resizable;
    _focus = w._focus;
    _size = w._size;
    _pos = w._pos;
    _alignment = w._alignment;
    _generateHandleAtEnterFocus = w._generateHandleAtEnterFocus;
    _margin = w._margin;
    _padding = w._padding;
    _percent = w._percent;
    _useStencil = w._useStencil;

	if (_widgetLook)
        delete _widgetLook;
    _widgetLook = (w._widgetLook) ? w._widgetLook->Clone() : NULL;

    _owner = NULL;
    _reposed = true;
    _resized = true;
    _stateChanged = true;
}

void    Widget::AddComposedWidget(Widget *widget)
{
    _composedWidget.push_back(widget);
    widget->_owner = this;
}

void    Widget::RemoveComposedWidget(Widget *widget)
{
    _composedWidget.remove(widget);
    widget->_owner = NULL;
}

bool    Widget::InhibitedRecursif() const
{
    Visitor::IsInhibited v;
    v(*this);
    return v.result;
}

void Widget::UpdateState()
{
    if (_widgetLook)
        _widgetLook->Update(_size);
}

void Widget::Resize()
{
    if (_resizable && (_percent[0] != 0 || _percent[1] != 0))
    {
        Visitor::GetParentWidget v(this);
        if (_owner != NULL)
            v.parent = _owner;
        else
            v(*this);

        Vector2i newSize = Size();

        Vector2i sizeParent;
        if (v.parent != NULL)
        {
            v.parent->SizeChild(this, sizeParent);
        }
        else if (v.parentSceneGraph != NULL) // get the size of the window attached to the scene graph which is supposed to be the parent of the widget at one point
        {
            Graphic::IWindow *win = v.parentSceneGraph->AttachedWindow();
            sizeParent.Init(win->Width(), win->Height());
        }
        else
        {
            throw Utils::Exception(Utils::Convert::ToString(*this), "Cannot find the scene graph attached to the widget, the widget should always be attached to the scene graph at one point.");
        }

        if (_percent[0] != 0)
            newSize[0] = ((float)(_percent[0] * sizeParent[0]) / 100.0);
        if (_percent[1] != 0)
            newSize[1] = ((float)(_percent[1] * sizeParent[1]) / 100.0);
        Size(newSize);
    }
}

void Widget::RenderBegin(Graphic::SceneGraph *scene)
{
    CheckState();

    // definit la position en fonction des corners
    Vector2i reelPos;
    RelativePos(reelPos);
    scene->PushModelMatrix();
    scene->ModelMatrix().AddTranslation(reelPos.data[0], reelPos.data[1], 0.f); // translation sur la position relative au Corner
}

void Widget::RenderEnd(Graphic::SceneGraph *scene)
{
    #ifdef _DEBUG_GUI // to draw the repere of the widget
    static Graphic::Object *repere = NULL;
    if (repere == NULL)
    {
        repere = Graphic::BasicMeshCreator::Axis(1);
        repere->Drawables()[0]->Config->RasterMode().SetLineWidth(2);
    }
    Vector2i reelSize;
    GetReelSize(reelSize);
    TMatrix m;
    m.Scale(reelSize.data[0], reelSize.data[1], 0);
    scene->ModelMatrix() *= m;
    repere->RenderNode(scene);
    #endif

    scene->PopModelMatrix();
}

void Widget::RenderChildsBegin(Graphic::SceneGraph *scene)
{
    if (_useStencil)
    {
        scene->GLState()->Enable(GL::Enum::ScissorTest);

        Vector2i pos, size(_size);
        AbsolutePos(pos);

        pos[0] += PaddingLeft();
        pos[1] += PaddingBottom();
        size[0] -= PaddingH();
        size[1] -= PaddingV();

        scene->GLState()->Scissor(pos[0], pos[1], size[0], size[1]);
    }
}

void Widget::RenderChildsEnd(Graphic::SceneGraph *scene)
{
    if (_useStencil)
        scene->GLState()->Disable(GL::Enum::ScissorTest);
}

void Widget::Draw(Graphic::SceneGraph *scene)
{
    // decorate the widget
    if (_widgetLook)
        _widgetLook->Draw(scene);
}

void Widget::ManageWindowEvent(const Event &event)
{
    if (event.type == Event::KeyPressed ||
        event.type == Event::KeyReleased)
        KeyboardEvent(event);
    else if (event.type == Event::MouseMoved)
        MouseMotionEvent(event);
    else if (event.type == Event::MouseButtonPressed ||
             event.type == Event::MouseButtonReleased ||
             event.type == Event::MouseWheelMoved)
        MouseButtonEvent(event);
    CheckFocus(event);
    if (_childFocused != NULL)
        _childFocused->ManageWindowEvent(event);
}

void Widget::CheckFocus(const Event &event)
{
    // testing focus childs
    if (event.type == Event::MouseButtonPressed && EnabledRecursif() && !InhibitedRecursif())
    {
        // set the last child to had the focus
        Widget *lastchildToHaveTheFocus = _childFocused;
        _childFocused = NULL;

        Vector2i mousePos = static_cast<WindowInput*>(event.emitter)->MousePositionInGLCoord();
        Visitor::CheckFocus v(event, mousePos);
        for (ListPWidget::const_iterator it = _composedWidget.begin(); it != _composedWidget.end(); ++it)
            v(**it);
        for(ContainerType::iterator it = _childs.begin(); v.childFocused == NULL && it != _childs.end(); it++)
            v(**it);
        _childFocused = v.childFocused;

        // unfocus the last child to had the focus
        if (lastchildToHaveTheFocus != NULL && lastchildToHaveTheFocus != _childFocused)
            lastchildToHaveTheFocus->Focus(false);
    }
}

void    Widget::Size(const Vector2i &size)
{
    if (_size != size)
    {
        _size = size;

        // notify the changement to every child
        Visitor::ResizedAll resizedAll;
        resizedAll(*this);

        _stateChanged = true;
    }
}

void    Widget::Pos(const Vector2i &pos)
{
    if (_pos != pos)
    {
        _pos = pos;

        // notify the changement to every child
        Visitor::ReposedAll reposedAll;
        reposedAll(*this);

        _stateChanged = true;
    }
}

void Widget::Reposed()
{
    _reposed = true;
}

void    Widget::RelativePos(Vector2i &relativePos) const
{
    Vector2i    parentSize, parentTranslate;
    BoxEdges    padding;

    relativePos = _pos;

    Visitor::GetParentWidget v(this);

    // if the widget is a composed widget, we'll take it's owner as parent
    if (_owner != NULL)
        v.parent = _owner;
    else
        v(*this);
    if (v.parent != NULL) // get the size of the widget parent
    {
        parentSize = v.parent->Size();
        padding = v.parent->_padding;
        if (v.parent->_widgetLook != NULL)
        {
            padding.left += v.parent->_widgetLook->edges.left;
            padding.right += v.parent->_widgetLook->edges.right;
            padding.top += v.parent->_widgetLook->edges.top;
            padding.bottom += v.parent->_widgetLook->edges.bottom;
        }
        v.parent->PosChild(this, parentTranslate);
    }
    else if (v.parentSceneGraph != NULL) // get the size of the window attached to the scene graph which is supposed to be the parent of the widget at one point
    {
        Graphic::IWindow *win = v.parentSceneGraph->AttachedWindow();
        parentSize.Init(win->Width(), win->Height());
    }
    else
    {
        throw Utils::Exception(Utils::Convert::ToString(*this), "Cannot find the scene graph attached to the widget, the widget should always be attached to the scene graph at one point.");
    }

    // check horizontal alignment
    if (_alignment.Enabled(Right))
        relativePos.data[0] = parentSize.data[0] - _size.data[0] - MarginRight() - padding.right - relativePos.data[0];
    else if (_alignment.Enabled(Left))
        relativePos.data[0] += MarginLeft() + padding.left;
    else if (_alignment.Enabled(CenterH))
        relativePos.data[0] += (parentSize[0] / 2.0) - (_size.data[0] / 2.0) + ((padding.left - padding.right) / 2);

    // check vertical alignment
    if (_alignment.Enabled(Top))
        relativePos.data[1] = parentSize.data[1] - _size.data[1] - MarginTop() - padding.top - relativePos.data[1];
    else if (_alignment.Enabled(Bottom))
        relativePos.data[1] += MarginBottom() + padding.bottom;
    else if (_alignment.Enabled(CenterV))
        relativePos.data[1] += (parentSize.data[1] / 2.0) - (_size.data[1] / 2.0) + ((padding.bottom - padding.top) / 2);
    relativePos += parentTranslate;
}

void    Widget::AbsolutePos(Vector2i &absolutePos) const
{
    // get back the first parent
    Visitor::GetParentWidget v(this);
    v(*this);
    if (v.parent != NULL)
        v.parent->AbsolutePos(absolutePos);
    Vector2i relativePos;
    RelativePos(relativePos);
    absolutePos += relativePos;
}

void Widget::Focus(bool state)
{
    if (_focus != state)
    {
        _focus = state;
        if (!state && _childFocused != NULL)
            _childFocused->Focus(false);
        if (_focus && _generateHandleAtEnterFocus && !_inhibit)
            SendEvent(_id);
        ChangeChildsStateRecursive();

        if (_focus)
            EnterFocus();
        else
            LeaveFocus();
    }
}

void Widget::CheckState()
{
    if (_reposed)
    {
        Repos();
        _reposed = false;
    }
    if (_resized)
    {
        Resize();
        _resized = false;
    }
    if (_stateChanged)
    {
        UpdateState();
        _stateChanged = false;
    }

    // check composed widget state
    for (ListPWidget::iterator it = _composedWidget.begin(); it != _composedWidget.end(); ++it)
        (*it)->CheckState();
}

void Widget::ChangeChildsStateRecursive()
{
    Visitor::ChangeStates v;
    v(*this);
}

void Widget::Percent(const Vector2f &percent)
{
    if (_percent != percent)
    {
        _percent = percent;

        // notify the changement to every childs
        Visitor::ResizedAll resizedAll;
        resizedAll(*this);
    }
}

void Widget::Resized()
{
    _resized = true;
    _stateChanged = true;
}

void    Widget::RemoveWidget(Widget *w)
{
    if (_childFocused == w)
        _childFocused = NULL;
    RemoveChild(w);
}

void    Widget::UseLook(GUI::ILook *look)
{
    if (_widgetLook)
        delete _widgetLook;
    _widgetLook = look;
}

void    Widget::SizeChild(const Widget *w, Vector2i &size) const
{
    size = _size;
    size[0] -= (PaddingLeft() + PaddingRight());
    size[1] -= (PaddingTop() + PaddingBottom());
    size[0] -= (w->MarginLeft() + w->MarginRight());
    size[1] -= (w->MarginTop() + w->MarginBottom());
}

void    Widget::Margin(const BoxEdges &margin)
{
    if (_margin != margin)
    {
        _margin = margin;
        _stateChanged = true;
    }
}

void    Widget::MarginH(int m)
{
    MarginLeft(m);
    MarginRight(m);
}

void    Widget::MarginV(int m)
{
    MarginTop(m);
    MarginBottom(m);
}

void    Widget::MarginLeft(int m)
{
    if (_margin.left != m)
    {
        _margin.left = m;
        _stateChanged = true;
    }
}

void    Widget::MarginRight(int m)
{
    if (_margin.right != m)
    {
        _margin.right = m;
        _stateChanged = true;
    }
}

void    Widget::MarginTop(int m)
{
    if (_margin.top != m)
    {
        _margin.top = m;
        _stateChanged = true;
    }
}

void    Widget::MarginBottom(int m)
{
    if (_margin.bottom != m)
    {
        _margin.bottom = m;
        _stateChanged = true;
    }
}

unsigned int    Widget::MarginH() const
{
    return MarginLeft() + MarginRight();
}

unsigned int    Widget::MarginV() const
{
    return MarginTop() + MarginBottom();
}

unsigned int    Widget::MarginLeft() const
{
    return _margin.left;
}

unsigned int    Widget::MarginRight() const
{
    return _margin.right;
}

unsigned int    Widget::MarginTop() const
{
    return _margin.top;
}

unsigned int    Widget::MarginBottom() const
{
    return _margin.bottom;
}

void    Widget::Padding(const BoxEdges &padding)
{
    if (_padding != padding)
    {
        _padding = padding;
        _stateChanged = true;
    }
}

void    Widget::PaddingH(int p)
{
    PaddingLeft(p);
    PaddingRight(p);
}

void    Widget::PaddingV(int p)
{
    PaddingTop(p);
    PaddingBottom(p);
}

void    Widget::PaddingLeft(int p)
{
    if (_padding.left != p)
    {
        _padding.left = p;
        _stateChanged = true;
    }
}

void    Widget::PaddingRight(int p)
{
    if (_padding.right != p)
    {
        _padding.right = p;
        _stateChanged = true;
    }
}

void    Widget::PaddingTop(int p)
{
    if (_padding.top != p)
    {
        _padding.top = p;
        _stateChanged = true;
    }
}

void    Widget::PaddingBottom(int p)
{
    if (_padding.bottom != p)
    {
        _padding.bottom = p;
        _stateChanged = true;
    }
}

unsigned int    Widget::PaddingH() const
{
    return PaddingLeft() + PaddingRight();
}

unsigned int    Widget::PaddingV() const
{
    return PaddingTop() + PaddingBottom();
}

unsigned int    Widget::PaddingLeft() const
{
    unsigned int r = _padding.left;
    return (_widgetLook != NULL) ? r + _widgetLook->edges.left : r;
}

unsigned int    Widget::PaddingRight() const
{
    unsigned int r = _padding.right;
    return (_widgetLook != NULL) ? r + _widgetLook->edges.right : r;
}

unsigned int    Widget::PaddingTop() const
{
    unsigned int r = _padding.top;
    return (_widgetLook != NULL) ? r + _widgetLook->edges.top : r;
}

unsigned int    Widget::PaddingBottom() const
{
    unsigned int r = _padding.bottom;
    return (_widgetLook != NULL) ? r + _widgetLook->edges.bottom : r;
}

void    Widget::Inhibit(bool status)
{
    _inhibit = status;
    ChangeChildsStateRecursive();
}

void    Widget::Inhibit()
{
    _inhibit = true;
    ChangeChildsStateRecursive();
}

void    Widget::Uninhibit()
{
    _inhibit = false;
    ChangeChildsStateRecursive();
}

void    Widget::GenerateHandleAtEnterFocus(bool state)
{
    _generateHandleAtEnterFocus = state;
    _stateChanged = true;
}

void    Widget::UseStencil(bool state)
{
    _useStencil = state;
    _stateChanged = true;
}

void    Widget::Enable(bool status)
{
    if (Enabled() != status)
    {
        Object::Enable(status);
        ChangeChildsStateRecursive();
    }
}

void    Widget::Enable()
{
    if (!Enabled())
    {
        Object::Enable();
        ChangeChildsStateRecursive();
    }
}

void    Widget::Disable()
{
    if (Enabled())
    {
        Object::Disable();
        ChangeChildsStateRecursive();
    }
}

