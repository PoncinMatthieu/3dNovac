
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

#ifndef POPULOUS_EDITOR_QTGAMEMANAGER_H_
#define POPULOUS_EDITOR_QTGAMEMANAGER_H_

#include "../Define.h"
#include "../Engines/QGraphicEngine.h"
#include <Nc/Core/Engine/Manager.h>

namespace Nc
{
  namespace Editor
  {
    class QManager : public QObject, public Nc::Engine::Manager
    {
        Q_OBJECT;

        public:
          QManager();
          virtual ~QManager();

        public slots:

        signals:
          void          SignalNewEngine(const std::string &name, Nc::Engine::IEngine *engine);
          void          SignalEngineDeleted(const std::string &name, Nc::Engine::IEngine *engine);

        public:
          virtual void  AddEngine(const std::string &name, Nc::Engine::IEngine *engine, const Permissions &permissions = Nc::Engine::Manager::Exit);
          virtual void  RemoveEngine(const std::string &name, bool del = true);
          virtual void  Start();
          virtual void  WaitEngines();
    };
  }
}

#endif
