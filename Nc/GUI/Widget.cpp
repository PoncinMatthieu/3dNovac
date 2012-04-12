
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
#include <Nc/Graphics/Object/Sprite.h>
#include "Widget.h"
#include "WidgetLabeled.h"
#include "Visitors.h"
#include "WindowStyle.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;

Widget::Widget(Corner x, Corner y, const Vector2i &pos, const Vector2i &size)
    : Object(), Handler()
{
    Init(pos, size, x, y);
}

void Widget::Init(const Vector2i &pos, const Vector2i &size, Corner x, Corner y)
{
    _childFocused = NULL;
    _inhibit = false;
    _focus = false;
    _generateHandleAtEnterFocus = false;
    _resizable = true;
    _size = size;
    _pos = pos;
    _corner[0] = x;
    _corner[1] = y;
    _useStencil = false;
    _margin = Vector2i(0, 0);
    _widgetLook = NULL;
    _owner = NULL;
    _stateChanged = true;
}

Widget::Widget(const Widget &w)
    : Object(w), Handler(w)
{
    Copy(w);
}

Widget &Widget::operator = (const Widget &w)
{
    Object::operator = (w);
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
    _corner[0] = w._corner[0];
    _corner[1] = w._corner[1];
    _generateHandleAtEnterFocus = w._generateHandleAtEnterFocus;
    _margin = w._margin;
    _percent = w._percent;
    _useStencil = w._useStencil;
    _widgetLook = (w._widgetLook) ? new Look(*w._widgetLook) : NULL;

    _owner = NULL;
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

void Widget::Update()
{
    Vector2i    size;
    GetReelSize(size);

    if (_widgetLook && _widgetLook->activated)
        _widgetLook->Update(size);
}

void Widget::RenderBegin(Graphic::SceneGraph *scene)
{
    CheckState();

    // definit la position en fonction des corners
    Vector2i reelPos;
    GetReelPos(reelPos);
    scene->PushModelMatrix();
    scene->ModelMatrix().AddTranslation(reelPos.Data[0], reelPos.Data[1], 0.f); // translation sur la position relative au Corner
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
    m.Scale(reelSize.Data[0], reelSize.Data[1], 0);
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

        Vector2i pos, size;
        GetReelPosRecursif(pos);
        GetReelSize(size);

        pos[0] += _margin[0];
        pos[1] += _margin[1];
        size[0] -= (_margin[0] * 2);
        size[1] -= (_margin[1] * 2);
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
    if (_widgetLook && _widgetLook->activated)
        _widgetLook->Draw(scene);
}

void Widget::ManageWindowEvent(const Event &event)
{
    if (event.Type == Event::KeyPressed ||
        event.Type == Event::KeyReleased)
        KeyboardEvent(event);
    else if (event.Type == Event::MouseMoved)
        MouseMotionEvent(event);
    else if (event.Type == Event::MouseButtonPressed ||
             event.Type == Event::MouseButtonReleased ||
             event.Type == Event::MouseWheelMoved)
        MouseButtonEvent(event);
    CheckFocus(event);
    if (_childFocused != NULL)
        _childFocused->ManageWindowEvent(event);
}

void Widget::CheckFocus(const Event &event)
{
    // testing focus childs
    if (event.Type == Event::MouseButtonPressed && EnabledRecursif() && !InhibitedRecursif())
    {
        // set the last child to had the focus
        Widget *lastchildToHaveTheFocus = _childFocused;
        _childFocused = NULL;

        Vector2i mousePos = WindowInput::MousePositionInGLCoord();
        Visitor::CheckFocus v(event, mousePos);
        for(ContainerType::iterator it = _childs.begin(); v.childFocused == NULL && it != _childs.end(); it++)
            v(**it);
        _childFocused = v.childFocused;

        // unfocus the last child to had the focus
        if (lastchildToHaveTheFocus != NULL && lastchildToHaveTheFocus != _childFocused)
            lastchildToHaveTheFocus->Focus(false);
    }
}

void Widget::GetReelPos(Vector2i &reelPos) const
{
    Vector2i    reelSize;
    Vector2i    parentSize(Window::Width(), Window::Height());
    Vector2i    parentTranslate;
    Vector2i    margin; // margin a 0, on prendra celui du parent s'il y en a un (margin interieur)

    GetReelSize(reelSize);
    reelPos = _pos;

    Visitor::GetParentWidget v(this);
    v(*this);
    if (v.parent != NULL) // recup la size et le margin du parent
    {
        v.parent->GetReelSize(parentSize);
        margin = v.parent->_margin;
        v.parent->PosChild(this, parentTranslate);
    }

    // check les corner en x
    if (_corner[0] == Right)
        reelPos.Data[0] = parentSize.Data[0] - reelSize.Data[0] - margin.Data[0] - reelPos.Data[0];
    else if (_corner[0] == Left)
        reelPos.Data[0] += margin.Data[0];
    else if (_corner[0] == Center)
        reelPos.Data[0] += (parentSize[0] / 2.0) - (reelSize.Data[0] / 2.0) + margin.Data[0];

    // check les corner en y
    if (_corner[1] == Top)
        reelPos.Data[1] = parentSize.Data[1] - reelSize.Data[1] - margin.Data[1] - reelPos.Data[1];
    else if (_corner[1] == Bottom)
        reelPos.Data[1] += margin.Data[1];
    else if (_corner[1] == Center)
        reelPos.Data[1] += (parentSize.Data[1] / 2.0) - (reelSize.Data[1] / 2.0) + margin.Data[1];
    reelPos += parentTranslate;
}

void Widget::GetReelPosRecursif(Vector2i &pos) const
{
    // get back the first parent
    Visitor::GetParentWidget v(this);
    v(*this);
    if (v.parent != NULL)
        v.parent->GetReelPosRecursif(pos);
    Vector2i reelPos;
    GetReelPos(reelPos);
    pos += reelPos;
}

void Widget::GetReelSize(Vector2i &size) const
{
    size = _size;
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
    }
}

void Widget::CheckState()
{
    if (_stateChanged)
    {
        Update();
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
    _percent = percent;
    Visitor::ResizeAll resizeAll;
    resizeAll(*this);
}

void Widget::Resized()
{
    if (_resizable && (_percent[0] != 0 || _percent[1] != 0))
    {
        Visitor::GetParentWidget v(this);
        v(*this);

        Vector2i newSize = Size();

        Vector2i sizeParent;
        if (v.parent != NULL)
            v.parent->SizeChild(this, sizeParent);
        else
            sizeParent.Init(Window::Width(), Window::Height());

        if (_percent[0] != 0)
            newSize[0] = ((float)(_percent[0] * sizeParent[0]) / 100.0);
        if (_percent[1] != 0)
            newSize[1] = ((float)(_percent[1] * sizeParent[1]) / 100.0);
        Size(newSize);
    }
    _stateChanged = true;
}

Widget::Look::Look(const std::string &name)
{
    // get the sprites from the window style (sprite name = name + position)
    _spriteLeftEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftEdge);
    _spriteTopEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetTopEdge);
    _spriteRightEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightEdge);
    _spriteBottomEdge = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetBottomEdge);
    _spriteLeftTop = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftTop);
    _spriteRightTop = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightTop);
    _spriteLeftBottom = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetLeftBottom);
    _spriteRightBottom = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetRightBottom);
    _spriteMiddle = WindowStyle::Instance().GetNewSprite(name + WindowStyle::SpriteName::WidgetMiddle);
    activated = true;
}

Widget::Look::Look(const Look &l)
{
    Copy(l);
}

Widget::Look &Widget::Look::operator = (const Look &l)
{
    Copy(l);
    return *this;
}

Widget::Look::~Look()
{
    if (_spriteLeftEdge != NULL)
        delete _spriteLeftEdge;
    if (_spriteTopEdge != NULL)
        delete _spriteTopEdge;
    if (_spriteRightEdge != NULL)
        delete _spriteRightEdge;
    if (_spriteBottomEdge != NULL)
        delete _spriteBottomEdge;
    if (_spriteLeftTop != NULL)
        delete _spriteLeftTop;
    if (_spriteRightTop != NULL)
        delete _spriteRightTop;
    if (_spriteLeftBottom != NULL)
        delete _spriteLeftBottom;
    if (_spriteRightBottom != NULL)
        delete _spriteRightBottom;
    if (_spriteMiddle != NULL)
        delete _spriteMiddle;
}

void    Widget::Look::Copy(const Look &l)
{
    activated = l.activated;

    _spriteLeftEdge = l._spriteLeftEdge;
    _spriteTopEdge = l._spriteTopEdge;
    _spriteRightEdge = l._spriteRightEdge;
    _spriteBottomEdge = l._spriteBottomEdge;
    _spriteLeftTop = l._spriteLeftTop;
    _spriteRightTop = l._spriteRightTop;
    _spriteLeftBottom = l._spriteLeftBottom;
    _spriteRightBottom = l._spriteRightBottom;
    _spriteMiddle = l._spriteMiddle;

    if (_spriteLeftEdge != NULL)
        _spriteLeftEdge = (Sprite*)_spriteLeftEdge->Clone();
    if (_spriteTopEdge != NULL)
        _spriteTopEdge = (Sprite*)_spriteTopEdge->Clone();
    if (_spriteRightEdge != NULL)
        _spriteRightEdge = (Sprite*)_spriteRightEdge->Clone();
    if (_spriteBottomEdge != NULL)
        _spriteBottomEdge = (Sprite*)_spriteBottomEdge->Clone();
    if (_spriteLeftTop != NULL)
        _spriteLeftTop = (Sprite*)_spriteLeftTop->Clone();
    if (_spriteRightTop != NULL)
        _spriteRightTop = (Sprite*)_spriteRightTop->Clone();
    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom = (Sprite*)_spriteLeftBottom->Clone();
    if (_spriteRightBottom != NULL)
        _spriteRightBottom = (Sprite*)_spriteRightBottom->Clone();
    if (_spriteMiddle != NULL)
        _spriteMiddle = (Sprite*)_spriteMiddle->Clone();
}

void Widget::UseLook(const std::string &name)
{
    if (_widgetLook)
        delete _widgetLook;
    _widgetLook = new Look(name);
}

void Widget::Look::Update(const Vector2i &size)
{
    int lxLeftBottom = 0, lyLeftBottom = 0, lxLeftTop = 0, lyLeftTop = 0, lxRightBottom = 0, lyRightBottom = 0, lxRightTop = 0, lyRightTop = 0;

    // get the corner size
    if (_spriteLeftBottom != NULL)
    {
        lxLeftBottom = _spriteLeftBottom->TextureBox().Length(0);
        lyLeftBottom = _spriteLeftBottom->TextureBox().Length(1);
    }
    if (_spriteLeftTop != NULL)
    {
        lxLeftTop = _spriteLeftTop->TextureBox().Length(0);
        lyLeftTop = _spriteLeftTop->TextureBox().Length(1);
    }
    if (_spriteRightBottom != NULL)
    {
        lxRightBottom = _spriteRightBottom->TextureBox().Length(0);
        lyRightBottom = _spriteRightBottom->TextureBox().Length(1);
    }
    if (_spriteRightTop != NULL)
    {
        lxRightTop = _spriteRightTop->TextureBox().Length(0);
        lyRightTop = _spriteRightTop->TextureBox().Length(1);
    }

    // Set the size and position of the sprites
    if (_spriteMiddle != NULL)
    {
        _spriteMiddle->Size(Vector2f(size[0] - lxLeftBottom - lxLeftTop, size[1] - lyLeftBottom - lyLeftTop));
        _spriteMiddle->Matrix.Translation(lxLeftBottom, lyLeftBottom, 0);
    }

    if (_spriteLeftTop != NULL)
    {
        _spriteLeftTop->Size(Vector2f(_spriteLeftTop->TextureBox().Length(0), lyLeftTop));
        _spriteLeftTop->Matrix.Translation(0, size[1] - lyLeftTop, 0);
    }

    if (_spriteRightTop != NULL)
    {
        _spriteRightTop->Size(Vector2f(lxRightTop, lyRightTop));
        _spriteRightTop->Matrix.Translation(size[0] - lxRightTop, size[1] - lyRightTop, 0);
    }

    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom->Size(Vector2f(_spriteLeftBottom->TextureBox().Length(0), _spriteLeftBottom->TextureBox().Length(1)));

    if (_spriteRightBottom)
    {
        _spriteRightBottom->Size(Vector2f(lxRightBottom, lyRightBottom));
        _spriteRightBottom->Matrix.Translation(size[0] - lxRightBottom, 0, 0);
    }

    if (_spriteLeftEdge != NULL)
    {
        _spriteLeftEdge->Size(Vector2f(_spriteLeftEdge->TextureBox().Length(0), size[1] - lyLeftBottom - lyLeftTop));
        _spriteLeftEdge->Matrix.Translation(0, lyLeftBottom, 0);
    }

    if (_spriteTopEdge != NULL)
    {
        float s = _spriteTopEdge->TextureBox().Length(1);
        _spriteTopEdge->Size(Vector2f(size[0] - lxLeftTop - lxRightTop, s));
        _spriteTopEdge->Matrix.Translation(lxLeftTop, size[1] - s, 0);
    }

    if (_spriteRightEdge != NULL)
    {
        float s = _spriteRightEdge->TextureBox().Length(0);
        _spriteRightEdge->Size(Vector2f(s, size[1] - lyRightBottom - lyRightTop));
        _spriteRightEdge->Matrix.Translation(size[0] - s, lyRightBottom, 0);
    }

    if (_spriteBottomEdge != NULL)
    {
        _spriteBottomEdge->Size(Vector2f(size[0] - lxLeftBottom - lxRightBottom, _spriteBottomEdge->TextureBox().Length(1)));
        _spriteBottomEdge->Matrix.Translation(lxLeftBottom, 0, 0);
    }
}

void Widget::Look::Draw(Graphic::SceneGraph *scene)
{
    scene->PushModelMatrix();

    // render middle sprite
    if (_spriteMiddle != NULL)
        _spriteMiddle->RenderNode(scene);

    // render corner sprites
    if (_spriteLeftTop != NULL)
        _spriteLeftTop->RenderNode(scene);
    if (_spriteRightTop != NULL)
        _spriteRightTop->RenderNode(scene);
    if (_spriteLeftBottom != NULL)
        _spriteLeftBottom->RenderNode(scene);
    if (_spriteRightBottom != NULL)
        _spriteRightBottom->RenderNode(scene);

    // render edge sprites
    if (_spriteLeftEdge != NULL)
        _spriteLeftEdge->RenderNode(scene);
    if (_spriteTopEdge != NULL)
        _spriteTopEdge->RenderNode(scene);
    if (_spriteRightEdge != NULL)
        _spriteRightEdge->RenderNode(scene);
    if (_spriteBottomEdge != NULL)
        _spriteBottomEdge->RenderNode(scene);

    scene->PopModelMatrix();
}

void    Widget::RemoveWidget(Widget *w)
{
    if (_childFocused == w)
        _childFocused = NULL;
    RemoveChild(w);
}
