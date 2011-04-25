
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
#include "Widget.h"
#include "WidgetLabeled.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::GUI;
using namespace Nc::Graphic;

Widget::Widget(const Vector2i &pos, const Vector2i &size, Corner x, Corner y, Widget *parent)
    : Handler(), _edgeColor(255, 0, 0), _margin(0, 0), _materialColored2d(&Material<BasicVertexType::Colored2d>::Instance())
{
    _childFocused = NULL;
    _enable = true;
    _focus = false;
    _drawEdge = false;
    _generateHandleAtEnterFocus = false;
    _size = size;
    _pos = pos;
    _corner[0] = x;
    _corner[1] = y;
    _parent = parent;
    if (_parent != NULL)
        _parent->AddChild(this); // ajoute this comme child au parent

    _drawableEdgeBox.GetVBO().Init(4, GL_STREAM_DRAW);
    _materialColored2d->Configure(_drawableEdgeBox);
    _drawableEdgeBox.SetPrimitiveType(GL_LINE_LOOP);
    _stateChange = true;
}

Widget::Widget(const Widget &w) : Handler(w)
{
    Copy(w);
}

Widget &Widget::operator = (const Widget &w)
{
    Copy(w);
    return *this;
}

Widget::~Widget()
{
    DeleteChilds();
}

void Widget::Copy(const Widget &w)
{
    _parent = w._parent;
    _childFocused = NULL;
    _enable = w._enable;
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

    _materialColored2d = w._materialColored2d;
    _drawableEdgeBox = w._drawableEdgeBox;

    for (std::list<Widget*>::const_iterator it = w._listChild.begin(); it != w._listChild.end(); ++it)
        _listChild.push_back((*it)->Clone());
    _stateChange = true;
}

void Widget::DeleteChilds()
{
    while (!_listChild.empty()) // free la liste de child
    {
        delete *(_listChild.begin());
        _listChild.pop_front();
    }
}

void Widget::AddChild(Widget *child)
{
    _listChild.push_back(child);
    _stateChange = true;
}

void Widget::Update()
{
    Array<BasicVertexType::Colored2d, 4>   vertices;
    Vector2f size = GetReelSize();
    vertices[0].Fill(0, 0, _edgeColor);
    vertices[1].Fill(0, size.Data[1], _edgeColor);
    vertices[2].Fill(size.Data[0], size.Data[1], _edgeColor);
    vertices[3].Fill(size.Data[0], 0, _edgeColor);
    _drawableEdgeBox.GetVBO().UpdateData(vertices.Data);
}

void Widget::Render(Graphic::ISceneGraph *scene)
{
// check les status
    CheckState();

// effectue le rendu
    // definit la position en fonction des corner
    Vector2f reelPos = GetReelPos();
    scene->PushModelMatrix();
        scene->ModelMatrix().AddTranslation(reelPos.Data[0], reelPos.Data[1], 0.f); // translation sur la position relative au Corner

        Draw(scene);

        // affichage des childs
        for (list<Widget*>::iterator it = _listChild.begin(); it != _listChild.end(); ++it)
            if ((*it)->_displayState)
                (*it)->Render(scene);

        #ifdef _DEBUG_GUI // dessine le coin de reference du widget
        static Graphic::Object *repere = NULL;
        if (repere == NULL)
            repere = Graphic::BasicMeshCreator::Repere(1);
        Vector2i reelSize = GetReelSize();
        scene->PushModelMatrix();
            TMatrix m;
            m.Scale(reelSize.Data[0], reelSize.Data[1], 0);
            scene->ModelMatrix() *= m;
        glLineWidth(2);
            repere->Render(scene);
        scene->PopModelMatrix();
        glLineWidth(1);
        #endif

    scene->PopModelMatrix();
}

void Widget::Draw(Graphic::ISceneGraph *scene)
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
    if (event.Type == Event::MouseButtonPressed) // test de focus
    {
        Widget *lastchildToHaveTheFocus = _childFocused;
        _childFocused = NULL;
        Vector2i mousePos = WindowInput::MousePositionInGLCoord();
        for(std::list<Widget*>::iterator it = _listChild.begin(); it != _listChild.end(); it++)
            if ((*it)->DisplayState())
            {
                Vector2f pos = (*it)->GetReelPosRecursif();
                Vector2f size = (*it)->GetReelSize();
    #ifdef _DEBUG_GUI_FOCUS
                LOG_DEBUG << "ReelPos   = " << pos << std::endl;
                LOG_DEBUG << "ReelSize  = " << size << std::endl;
                LOG_DEBUG << "Mouse = " << mousePos << std::endl;
                LOG_DEBUG << std::endl;
    #endif
                if (Math::InRect(pos, size, mousePos))
                {
                    _childFocused = *it;
                    _childFocused->Focus(true);
                }
            }
        if (lastchildToHaveTheFocus != NULL && lastchildToHaveTheFocus != _childFocused)
            lastchildToHaveTheFocus->Focus(false);
    }
}

Vector2f Widget::GetReelPos() const
{
    Vector2f    reelSize = GetReelSize();
    Vector2f    reelPos(_pos);
    Vector2f    parentSize(Window::Width(), Window::Height());
    Vector2f    parentTranslate;
    Vector2i    margin; // margin a 0, on prendra celui du parent s'il y en a un (margin interieur)

    if (_parent != NULL) // recup la size et le margin du parent
    {
        parentSize = _parent->GetReelSize();
        margin = _parent->_margin;
        parentTranslate = _parent->TranslateChild(_corner);
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
    return reelPos;
}

Vector2f Widget::GetReelPosRecursif() const
{
    Vector2f pos;
    if (_parent != NULL)
        pos += _parent->GetReelPosRecursif();
    pos += GetReelPos();
    return pos;
}

Vector2f Widget::GetReelSize() const
{
    return _size;
}

void Widget::Focus(bool state)
{
    if (_focus != state)
    {
        _focus = state;
        if (_childFocused != NULL)
            _childFocused->Focus(state);
        if (_focus && _generateHandleAtEnterFocus && _displayState)
            SendEvent(_id);
        _stateChange = true;
    }
}

void Widget::CheckState()
{
    for (list<Widget*>::iterator it = _listChild.begin(); it != _listChild.end(); it++)
        (*it)->CheckState();
    if (_stateChange)
    {
        Update();
        if (_parent != NULL)
            _parent->_stateChange = true;
        _stateChange = false;
    }
}

void Widget::GetParentChildData(list<string> &listData)
{
    string s;

    if (_parent != NULL)
    {
        for (list<Widget*>::iterator it = _parent->_listChild.begin(); it != _parent->_listChild.end(); it++)
        {
            (*it)->GetData(s);
            if (!s.empty())
                listData.push_back(s);
        }
    }
}

void Widget::Percent(Vector2f percent)
{
    _percent = percent;
    Resized();
}

void Widget::Resized()
{
    float sParent;
    if (_percent.Data[0] != 0)
    {
        sParent = (_parent == NULL) ? Window::Width() : _parent->_size[0];
        _size[0] = ((float)(_percent.Data[0] * sParent) / 100.0);
    }
    if (_percent.Data[1] != 0)
    {
        sParent = (_parent == NULL) ? Window::Height() : _parent->_size[1];
        _size[1] = ((float)(_percent.Data[1] * sParent) / 100.0);
    }
    for (list<Widget*>::iterator it = _listChild.begin(); it != _listChild.end(); it++)
        (*it)->Resized();
    _stateChange = true;
}

void Widget::DrawEdgeBox(Graphic::ISceneGraph *scene)
{
    _materialColored2d->Render(scene, _drawableEdgeBox);
}
