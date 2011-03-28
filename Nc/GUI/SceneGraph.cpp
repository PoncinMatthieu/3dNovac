
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

#include "SceneGraph.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace Nc;
using namespace Nc::Graphic;
using namespace Nc::GUI;

SceneGraph::SceneGraph(Camera2d *camera) : I2dSceneGraph(camera)
{
    _widgetFocused = NULL;
}

SceneGraph::~SceneGraph()
{
}

void SceneGraph::RenderIn2D()
{
    _mutex.Lock();
// Draw les object2d
    for(ListPObject::iterator it = _listObject.begin(); it != _listObject.end(); it++)
        if ((*it)->DisplayState())
            (*it)->Render(this);
// Draw les widget
    for(ListPWidget::iterator it = _listWidget.begin(); it != _listWidget.end(); it++)
        if ((*it)->DisplayState())
            (*it)->Render(this);
    _mutex.Unlock();
}

void SceneGraph::Clear(bool del)
{
    if (del)
    {
        Utils::DeleteContainer(_listWidget);
        Utils::DeleteContainer(_listObject);
    }
    else
    {
        _listWidget.clear();
        _listObject.clear();
    }
}

void SceneGraph::DeleteObject(Graphic::Object *object, bool del)
{
    _mutex.Lock();
    _listObject.remove(object);
    Widget *widget = dynamic_cast<Widget*>(object);
    if (widget != NULL)
        _listWidget.remove(widget);
    if (widget == _widgetFocused)
        _widgetFocused = NULL;
    if (del)
        delete object;
    _mutex.Unlock();
}

void SceneGraph::AddObject(Graphic::Object *object)
{
    _mutex.Lock();
    _listObject.push_back(object);
    _mutex.Unlock();
}

void SceneGraph::AddWidget(Widget *widget)
{
    _mutex.Lock();
    if (widget->Parent() != NULL)
        AddWidget(widget->Parent());
    else
        _listWidget.push_back(widget);
    _mutex.Unlock();
}

void SceneGraph::BringToFront(Widget *w)
{
    if (w != NULL)
    {
        _mutex.Lock();
        std::list<Widget*>::iterator it = find(_listWidget.begin(), _listWidget.end(), w);
        if (it != _listWidget.end())
            _listWidget.erase(it);
        _listWidget.push_back(w);
        _mutex.Unlock();
    }
}

void SceneGraph::ManageWindowEvent(const System::Event &event)
{
    _mutex.Lock();
// update en cas de resize de la fenetre
    if (event.Type == System::Event::Resized)
    {
        _camera->Resized(event);
        for(std::list<Widget*>::iterator it = _listWidget.begin(); it != _listWidget.end(); it++)
            (*it)->Resized();
    }

// test de focus
    if (event.Type == System::Event::MouseButtonPressed)
    {
        Widget *lastWidgetToHaveTheFocus = _widgetFocused;
        _widgetFocused = NULL;
        Vector2i mousePos = WindowInput::MousePositionInGLCoord();
        for(std::list<Widget*>::iterator it = _listWidget.begin(); it != _listWidget.end(); ++it)
        {
            #ifdef _DEBUG_GUI_FOCUS
            LOG_DEBUG << "ReelPos   = " << (*it)->GetReelPos() << std::endl;
            LOG_DEBUG << "ReelSize  = " << (*it)->GetReelSize() << std::endl;
            LOG_DEBUG << "Mouse = " << mousePos << std::endl;
            LOG_DEBUG << std::endl;
            #endif
            if ((*it)->DisplayState() && (Math::InRect((*it)->GetReelPos(), (*it)->GetReelSize(), mousePos)))
            {
                _widgetFocused = *it;
                _widgetFocused->Focus(true);
            }
        }
        if (lastWidgetToHaveTheFocus != NULL && lastWidgetToHaveTheFocus != _widgetFocused)
            lastWidgetToHaveTheFocus->Focus(false);
    }
// send les event au widget qui a le focus
    if (_widgetFocused != NULL)
        _widgetFocused->ManageWindowEvent(event);
    _mutex.Unlock();
}
