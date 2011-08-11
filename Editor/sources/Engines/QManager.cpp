
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

#include "QManager.h"

using namespace std;
using namespace Nc;
using namespace Nc::System;
using namespace Nc::Editor;

QManager::QManager()
    : QObject(), Nc::Engine::Manager()
{
}

QManager::~QManager()
{
}

void QManager::AddEngine(const std::string &name, Nc::Engine::IEngine *engine, const Permissions &permissions)
{
    Nc::Engine::Manager::AddEngine(name, engine, permissions);
    emit SignalNewEngine(name, engine);
}

void  QManager::RemoveEngine(const std::string &name, bool del)
{
    emit SignalEngineDeleted(name, GetEngine(name));
    Nc::Engine::Manager::RemoveEngine(name, del);
}

void QManager::Start()
{
    // lancement des threads
    _isLaunched = true;
    for (Nc::Engine::Manager::MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); ++itEngine)
        if (itEngine->first !=  "Graphic") // dans Qt notre graphic engine n'aura pas de thread dedie
            itEngine->second.engine->Start();
}

void QManager::WaitEngines()
{
// on attend que les threads se finisse a l'exception du graphic engine
    for (Nc::Engine::Manager::MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); ++itEngine)
        if (itEngine->first !=  "Graphic")
        itEngine->second.engine->Wait();
}
