
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
#include "Visitors.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace Nc;
using namespace Nc::GUI;

SceneGraph::SceneGraph(Graphic::Window *attachedWindow)
    : Graphic::SceneGraph(true),
      _widgetFocused(NULL), _attachedWindow(attachedWindow)
{
}

SceneGraph::~SceneGraph()
{
}

SceneGraph::SceneGraph(const SceneGraph &scene)
    : Graphic::SceneGraph(scene), _widgetFocused(NULL)
{
}

SceneGraph &SceneGraph::operator = (const SceneGraph &scene)
{
    Graphic::SceneGraph::operator = (scene);
    _widgetFocused = NULL;
    return *this;
}

void SceneGraph::BringToFront(Widget *w)
{
    if (w != NULL)
    {
        Lock();
        AddChild(w);        // add to the end
        RemoveChild(w);     // remove the first occurence
        Unlock();
    }
}

void SceneGraph::ManageWindowEvent(const System::Event &event)
{
// update en cas de resize de la fenetre
    if (event.type == System::Event::Resized)
    {
        if (_currentCamera != NULL)
            _currentCamera->Resized(event);

        Visitor::ResizeAll resizeAll;
        resizeAll(*this);
    }

// test de focus
    if (event.type == System::Event::MouseButtonPressed)
    {
        Widget *lastWidgetToHaveTheFocus = _widgetFocused;
        _widgetFocused = NULL;
        Vector2i mousePos = Graphic::WindowInput::MousePositionInGLCoord(static_cast<Graphic::WindowInput*>(event.emitter)->AttachedWindow()->Height());
        for(ContainerType::reverse_iterator it = _childs.rbegin(); _widgetFocused == NULL && it != _childs.rend(); it++)
        {
            Widget *w = (*it)->AsWithoutThrow<Widget>();
			if (w != NULL && w->Enabled() && !w->Inhibited())
            {
                Vector2i    reelPos;
                Vector2i    reelSize;
                w->GetReelPos(reelPos);
                w->GetReelSize(reelSize);
                #ifdef _DEBUG_GUI_FOCUS
                LOG_DEBUG << "Widget: " << *w << std::endl;
                LOG_DEBUG << "ReelPos   = " << reelPos << std::endl;
                LOG_DEBUG << "ReelSize  = " << reelSize << std::endl;
                LOG_DEBUG << "Mouse = " << mousePos << std::endl;
                #endif
                if (Math::InRect(reelPos, reelSize, mousePos))
                {
                    _widgetFocused = w;
                    _widgetFocused->Focus(true);
                    #ifdef _DEBUG_GUI_FOCUS
                    LOG_DEBUG << "OK" << std::endl;
                    #endif
                }
                #ifdef _DEBUG_GUI_FOCUS
                LOG << std::endl;
                #endif
            }
        }
        if (lastWidgetToHaveTheFocus != NULL && lastWidgetToHaveTheFocus != _widgetFocused)
            lastWidgetToHaveTheFocus->Focus(false);
    }
// send les event au widget qui a le focus
    if (_widgetFocused != NULL)
        _widgetFocused->ManageWindowEvent(event);
}

void    SceneGraph::RemoveWidget(Widget *w)
{
    if (_widgetFocused == w)
        _widgetFocused = NULL;
    RemoveChild(w);
}
