
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
#include "WidgetLabeled.h"
#include "Visitors.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;

Widget::Widget(const Vector2i &pos, const Vector2i &size, Corner x, Corner y)
    : Object(ClassName()), Handler(), _edgeColor(255, 0, 0), _margin(0, 0)
{
    Init(pos, size, x, y);
}

Widget::Widget(const char *className, const Vector2i &pos, const Vector2i &size, Corner x, Corner y)
    : Object(className), Handler(), _edgeColor(255, 0, 0), _margin(0, 0)
{
    Init(pos, size, x, y);
}

void Widget::Init(const Vector2i &pos, const Vector2i &size, Corner x, Corner y)
{
    _childFocused = NULL;
    _inhibit = false;
    _focus = false;
    _drawEdge = false;
    _generateHandleAtEnterFocus = false;
    _size = size;
    _pos = pos;
    _corner[0] = x;
    _corner[1] = y;

    // creation du drawable
    GL::GeometryBuffer<DefaultVertexType::Colored2d, false> *geometry = new GL::GeometryBuffer<DefaultVertexType::Colored2d, false>(GL::Enum::LineLoop);
    geometry->VBO().Init(4, GL::Enum::DataBuffer::StreamDraw);
    _drawables.push_back(new Drawable(geometry));
    ChooseDefaultMaterial();
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
}

void Widget::Copy(const Widget &w)
{
    _childFocused = NULL;
    _inhibit = w._inhibit;
    _focus = w._focus;
    _size = w._size;
    _pos = w._pos;
    _corner[0] = w._corner[0];
    _corner[1] = w._corner[1];
    _drawEdge = w._drawEdge;
    _generateHandleAtEnterFocus = w._generateHandleAtEnterFocus;
    _edgeColor = w._edgeColor;
    _margin = w._margin;
    _percent = w._percent;
    _stateChanged = true;
}

bool    Widget::InhibitedRecursif() const
{
    Visitor::IsInhibited v;
    v(*this);
    return v.result;
}

void Widget::Update()
{
    Array<DefaultVertexType::Colored2d, 4>  vertices;
    Vector2f                                size;

    GetReelSize(size);
    vertices[0].Fill(0, 0, _edgeColor);
    vertices[1].Fill(0, size.Data[1], _edgeColor);
    vertices[2].Fill(size.Data[0], size.Data[1], _edgeColor);
    vertices[3].Fill(size.Data[0], 0, _edgeColor);
    static_cast<GL::GeometryBuffer<DefaultVertexType::Colored2d, false>*>(_drawables[0]->Geometry)->VBO().UpdateData(vertices.Data);
}

void Widget::Render(Graphic::SceneGraph *scene)
{
    if (!_enabled)
        return;

    CheckState();

// effectue le rendu
    // definit la position en fonction des corner
    Vector2f reelPos;
    GetReelPos(reelPos);
    scene->PushModelMatrix();
        scene->ModelMatrix().AddTranslation(reelPos.Data[0], reelPos.Data[1], 0.f); // translation sur la position relative au Corner

        Draw(scene);

        // affichage des childs
        RenderChilds(scene);

        #ifdef _DEBUG_GUI // to draw the repere of the widget
        static Graphic::Object *repere = NULL;
        if (repere == NULL)
        {
            repere = Graphic::BasicMeshCreator::Repere(1);
            repere->Drawables()[0]->Config->RasterMode().SetLineWidth(2);
        }
        Vector2f reelSize;
        GetReelSize(reelSize);
        TMatrix m;
        m.Scale(reelSize.Data[0], reelSize.Data[1], 0);
        scene->ModelMatrix() *= m;
        repere->Render(scene);
        #endif

    scene->PopModelMatrix();
}

void Widget::Draw(Graphic::SceneGraph *scene)
{
#ifndef _DEBUG_GUI_BOX
    if (_drawEdge) // dessine la box du widget
#endif
    {
        DrawEdgeBox(scene);
    }
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

void Widget::GetReelPos(Vector2f &reelPos) const
{
    Vector2f    reelSize;
    Vector2f    parentSize(Window::Width(), Window::Height());
    Vector2f    parentTranslate;
    Vector2i    margin; // margin a 0, on prendra celui du parent s'il y en a un (margin interieur)

    GetReelSize(reelSize);
    reelPos = _pos;

    Visitor::GetParentWidget v(this);
    v(*this);
    if (v.parent != NULL) // recup la size et le margin du parent
    {
        v.parent->GetReelSize(parentSize);
        margin = v.parent->_margin;
        v.parent->TranslateChild(_corner, parentTranslate);
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

void Widget::GetReelPosRecursif(Vector2f &pos) const
{
    // get back the first parent
    Visitor::GetParentWidget v(this);
    v(*this);
    if (v.parent != NULL)
        v.parent->GetReelPosRecursif(pos);
    Vector2f reelPos;
    GetReelPos(reelPos);
    pos += reelPos;
}

void Widget::GetReelSize(Vector2f &size) const
{
    size = _size;
}

void Widget::Focus(bool state)
{
    if (_focus != state)
    {
        _focus = state;
        if (!state && _childFocused != NULL)
            _childFocused->Focus(state);
        if (_focus && _generateHandleAtEnterFocus && !_inhibit)
            SendEvent(_id);

        //_stateChanged = true;
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
}

void Widget::ChangeChildsStateRecursive()
{
    Visitor::ChangeStates v;
    v(*this);
}

void Widget::Percent(const Vector2f &percent)
{
    _percent = percent;
    Resized();
}

void Widget::Resized()
{
    float sParent;
    if (Percent().Data[0] != 0 || Percent().Data[1] != 0)
    {
        Visitor::GetParentWidget v(this);
        v(*this);

        Vector2i newSize = Size();
        if (Percent().Data[0] != 0)
        {
            sParent = (v.parent != NULL) ? v.parent->Size()[0] : Window::Width();
            newSize[0] = ((float)(Percent().Data[0] * sParent) / 100.0);
        }
        if (Percent().Data[1] != 0)
        {
            sParent = (v.parent != NULL) ? v.parent->Size()[1] : Window::Height();
            newSize[1] = ((float)(Percent().Data[1] * sParent) / 100.0);
        }
        Size(newSize);
    }
    ChangeState();
}

void Widget::DrawEdgeBox(Graphic::SceneGraph *scene)
{
    GetMaterial()->Render(scene, *_drawables[0]);
}
