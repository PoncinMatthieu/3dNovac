
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
#include "Visitor/Visitors.h"
#include <Nc/Core/Utils/Debug/OverloadAlloc.h>

using namespace Nc;
using namespace Nc::GUI;

SceneGraph::SceneGraph(Graphic::IWindow *attachedWindow)
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
        System::Locker l(&GetMutex());
        AddChild(w);        // add to the end
        RemoveChild(w);     // remove the first occurence
    }
}

struct DispatchEvents : public Visitor::WidgetVisitor<DispatchEvents, false, void>
{
    DispatchEvents(const Nc::System::Event &e)
        : WidgetVisitor<DispatchEvents, false, void>(   Utils::Metaprog::Seq<Widget>::Type(),
                                                        Utils::Metaprog::Seq<Widget, Graphic::Entity, Graphic::Octree>::Type()),
          event(e)
    {
    }

    void    VisitNode(Widget &w)
    {
        if (w.AlwaysReceiveEvents() || w.Focus())
            w.ManageWindowEvent(event);
    }

    const Nc::System::Event     &event;
};

void SceneGraph::ManageWindowEvent(const System::Event &event)
{
// update en cas de resize de la fenetre
    if (event.type == System::Event::Resized)
    {
        if (_currentCamera != NULL)
            _currentCamera->Resized(Vector2ui(event.size.width, event.size.height));

        // notify the resize to every widget
        Visitor::ResizedAll childResized;
        childResized(*this);
    }

// test de focus
    if (event.type == System::Event::MouseButtonPressed)
    {
        Widget *lastWidgetToHaveTheFocus = _widgetFocused;
        _widgetFocused = NULL;
        Vector2i mousePos = static_cast<Graphic::WindowInput*>(event.emitter)->MousePositionInGLCoord();
        for(ContainerType::reverse_iterator it = _childs.rbegin(); _widgetFocused == NULL && it != _childs.rend(); it++)
        {
            Widget *w = (*it)->AsWithoutThrow<Widget>();
			if (w != NULL && Widget::TestFocus(*w, mousePos))
            {
                _widgetFocused = w;
                _widgetFocused->Focus(true);
            }
        }
        if (lastWidgetToHaveTheFocus != NULL && lastWidgetToHaveTheFocus != _widgetFocused)
            lastWidgetToHaveTheFocus->Focus(false);
    }

/*
// test focus
    if (event.type == System::Event::MouseButtonPressed)
    {
        Widget *lastWidgetToHaveTheFocus = _widgetFocused;
        Visitor::TestFocus focus(this, event, static_cast<Graphic::WindowInput*>(event.emitter)->MousePositionInGLCoord());
        focus(*this);
        _widgetFocused = focus.childFocused;
        if (lastWidgetToHaveTheFocus != NULL && lastWidgetToHaveTheFocus != _widgetFocused)
            lastWidgetToHaveTheFocus->Focus(false);
    }
*/
// dispatch events to the widgets
    DispatchEvents dispatch(event);
    dispatch(*this);
}

void    SceneGraph::RemoveWidget(Widget *w)
{
    if (_widgetFocused == w)
        _widgetFocused = NULL;
    RemoveChild(w);
}
