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

#ifndef POPULOUS_EDITOR_QGRAPHIC_ENGINE_H_
#define POPULOUS_EDITOR_QGRAPHIC_ENGINE_H_

#include <GL/glew.h>
#include <QtGui>

#include "Define.h"
#include <Nc/Graphics/Engine.h>

#include <GL/glx.h>
#include <GL/gl.h>
#include <GL/glu.h>
#if defined(Q_WS_X11)
#include <QtGui/QX11Info>
#endif

namespace Nc
{
  namespace Editor
  {
    class QManager;

    class QGraphicEngine : public QWidget, public Nc::Graphic::Engine
    {
    public:
      QGraphicEngine(Nc::Engine::Manager *gameManager);
      virtual ~QGraphicEngine();

    protected:
      void CreateContext();

      /// manage the inputs of Qt
      bool	        x11Event(XEvent *event);

      /// manage the render window
      void            showEvent(QShowEvent *e);
      void            paintEvent(QPaintEvent *e);
      QPaintEngine    *paintEngine() const;

    private:
      virtual void    Run() {} 	// the main loop is manage by Qt

      bool            _windowCreated;
    };
  }
}

#endif
