
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
using namespace Nc::GUI;

SceneGraph::SceneGraph()
    : Graphic::SceneGraph(ClassName(), true),
      _widgetFocused(NULL)
{
}

SceneGraph::SceneGraph(const char *className)
    : Graphic::SceneGraph(className, true),
      _widgetFocused(NULL)
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

struct FonctorResized
{
    bool operator () (Graphic::ISceneNode *node)
    {
        Widget *w = node->AsWithoutThrow<Widget>();
        if (w != NULL)
        {
            w->Resized();
            return false;
        }
        return true;
    }
};

void SceneGraph::ManageWindowEvent(const System::Event &event)
{
// update en cas de resize de la fenetre
    if (event.Type == System::Event::Resized)
    {
        if (_currentCamera != NULL)
            _currentCamera->Resized(event);
        FonctorResized f;
        for(ContainerType::iterator it = _childs.begin(); it != _childs.end(); it++)
            (*it)->ForEachChilds<false>(f);
    }

// test de focus
    if (event.Type == System::Event::MouseButtonPressed)
    {
        Widget *lastWidgetToHaveTheFocus = _widgetFocused;
        _widgetFocused = NULL;
        Vector2i mousePos = Graphic::WindowInput::MousePositionInGLCoord();
        for(ContainerType::iterator it = _childs.begin(); _widgetFocused == NULL && it != _childs.end(); it++)
        {
            Widget *w = (*it)->AsWithoutThrow<Widget>();
            if (w != NULL)
            {
                //w->Resized();
                Vector2f    reelPos;
                Vector2f    reelSize;
                w->GetReelPos(reelPos);
                w->GetReelSize(reelSize);
                #ifdef _DEBUG_GUI_FOCUS
                LOG_DEBUG << "Widget: " << *w << std::endl;
                LOG_DEBUG << "ReelPos   = " << reelPos << std::endl;
                LOG_DEBUG << "ReelSize  = " << reelSize << std::endl;
                LOG_DEBUG << "Mouse = " << mousePos << std::endl;
                #endif
                if (w->DisplayState() && (Math::InRect(reelPos, reelSize, mousePos)))
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
