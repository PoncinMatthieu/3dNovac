
/*-----------------------------------------------------------------------------

	3dNovac Editor
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

    File Created At:        05/12/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "QGraphicEngine.h"
#include "QManager.h"
#include <Nc/Graphics/Window/Window/XWindow.h>
#include <Nc/Graphics/Window/Input/XWindowInput.h>
#include "QSceneGraphManager.h"

using namespace std;
using namespace Nc;
using namespace Nc::Editor;

QGraphicEngine::QGraphicEngine(Nc::Engine::Manager *manager)
    : QWidget(NULL), Graphic::Engine(manager, NULL, new QSceneGraphManager()), _windowCreated(false)
{
// Qt doc:
// Indicates that the widget paints all its pixels when it receives a paint event.
// Thus, it is not required for operations like updating, resizing, scrolling and focus changes to erase the widget before generating paint events.
// The use of WA_OpaquePaintEvent provides a small optimization by helping to reduce flicker on systems that do not support double buffering and avoiding computational cycles necessary to erase the background prior to painting.
    setAttribute(Qt::WA_OpaquePaintEvent);

// Qt doc:
// Indicates that the widget wants to draw directly onto the screen.
// Widgets with this attribute set do not participate in composition management, i.e.
// they cannot be semi-transparent or shine through semi-transparent overlapping widgets.
// Note: This flag is only supported on X11 and it disables double buffering.
// if you require native painting primitives, you need to reimplement QWidget::paintEngine() to return 0 and set this flag.
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoFillBackground(true); // for compatibility

    setMinimumSize(200, 200);
}

QGraphicEngine::~QGraphicEngine()
{
}

void QGraphicEngine::CreateContext()
{
    // create the xwindow and tel it that we use an existing windows
    _win = new Graphic::XWindow();
    _win->UseExistingWindow(x11Info().display(), winId(), Vector2i(width(), height()), 0);
    _context = _win->CreateGLContext();

    // activation du context opengl
    ActiveContext();

    // initialise opengl
    _renderState.InitContext(_context);
    _renderState.Enable();
    _sceneGraphManager->Init((_win->AntialiasingLevel() > 0));
    _renderState.Disable();
    _windowCreated = true;
}

void QGraphicEngine::showEvent(QShowEvent *e)
{
    // creation de la fenetre lors du premier showEvent
    if (!_windowCreated)
    {
        _manager->WaitLoadingContextPriority(_loadingContextPriority); // waiting for our turn to load the context

        _manager->MutexGlobal().Lock();
        try
        {
            if (_pattern.Enabled(Nc::Engine::HasAContext))
            {
                LOG_DEBUG << "---------------"<< *this << "-:-" << "Create Context-------------------" << endl;
                CreateContext();
                DisableContext();
                //_pattern.Enable(Nc::Engine::ContextIsLoaded); // ressource temporarely anavailable if it's done here
                LOG_DEBUG << "---------------"<< *this << "-:-" << "Create Context DONE--------------" << endl;
            }
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR during creating context : " << e.what() << std::endl;
            exit(-1);
        }
       _manager->MutexGlobal().Unlock();
    }
    QWidget::showEvent(e);
}

void QGraphicEngine::paintEvent(QPaintEvent *e)
{
    QWidget::paintEvent(e);
    if (_loaded)
        Process();
}

QPaintEngine *QGraphicEngine::paintEngine() const
{
    return 0;
}

bool QGraphicEngine::x11Event(XEvent *event)
{
// au premier evenement de la fenetre (ce la veut dire que la fenetre est bien load),
// on peut indiquer que le context est bien cree, puis on attent que les autres engines creer leurs context puis on load les contents
    if (_windowCreated)
    {
    // des que l'on recoit un event on set le focus sur le widget, comme ca si la souris pointe sur le widget on peut utiliser le clavier
    // (par defaut dans Qt, les widgets recoive les evenements de la souris mais pas du clavier)
        if (!hasFocus())
            setFocus();

        if (!_loaded)
        {
            _pattern.Enable(Nc::Engine::ContextIsLoaded);           // on the first x11Event we set context loaded statement
            _manager->WaitEnginesContextLoading();	                // waiting for others context
            _manager->WaitLoadingPriority(_loadingPriority);        // waiting for our turn to load contents

            _manager->MutexGlobal().Lock();
            try
            {
                ActiveContext();
                LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-----------------------" << endl;
                LoadContent();
                LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-done------------------" << endl;
                _loaded = true;
                DisableContext();
            }
            catch (const std::exception &e)
            {
                LOG << "FATAL ERROR during Loading contents : " << e.what() << std::endl;
                exit(-1);
            }
            _manager->MutexGlobal().Unlock();
            if (_pattern.Enabled(Nc::Engine::WaitingLoadContentsOfOthersEngines))
                _manager->WaitEnginesLoading();                     // waiting for others loading content engines

            // active the gl context
            ActiveContext();
        }

    // on active notre context opengl et on send les event a notre windowInput
        _manager->MutexGlobal().Lock();
        static_cast<Graphic::XWindowInput*>(_win->GetInput())->ProcessEvent(*event);
        _manager->MutexGlobal().Unlock();
    }
    return QWidget::x11Event(event);    // send les event au widget sinon, on a des erreur x11 d'evenement non traite
}
