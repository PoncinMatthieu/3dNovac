
/*-----------------------------------------------------------------------------

	3dNovac Core
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

    File Created At:        24/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#include "Manager.h"
#include "../System/Config.h"

using namespace std;
using namespace Nc;
using namespace Nc::Engine;

Manager::MapEngine      Manager::_mapEngine;
System::Mutex           Manager::_mutexGlobal;

Manager::Manager(const Utils::FileName &confFile, bool initCrashReporter)
{
    CALLSTACK_INFO();

    _mainThreadId = System::ThreadId();
    _isLaunched = false;

    if (initCrashReporter)
    {
        Utils::CrashReporter::Instance().Init();
        _crashReporterInitialized = true;
    }

    // Open the Config file of 3dNovac
    if (!confFile.empty())
    {
        CONFIG.Load(confFile);
        _confFileOpened = true;
    }
}

Manager::~Manager()
{
    CALLSTACK_INFO();

// priority destruction ! (if 0, no delete)
    unsigned char lowestPriority = 0xff;
    unsigned char nextPriority;

    // first find the lowest priority
    for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
        if (it->second.engine->DeletePriority() < lowestPriority && it->second.engine->DeletePriority() > 0)
            lowestPriority = it->second.engine->DeletePriority();

    // and next delete all engines with the priority order
    while (lowestPriority != 0 && _mapEngine.size() > 0)
    {
        nextPriority = 0xff;
        MapEngine::iterator it2;
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); it = it2)
        {
            it2 = it;
            ++it2;
            if (it->second.engine->DeletePriority() == lowestPriority)
            {
                delete it->second.engine;
                _mapEngine.erase(it);
            }
            else if (it->second.engine->DeletePriority() < nextPriority)
                nextPriority = it->second.engine->DeletePriority();
        }
        lowestPriority = nextPriority;
    }

// close the Config file of 3dNovac
    if (_confFileOpened)
        CONFIG.DeleteInstance();

// delete the crash reporter instance
    if (_crashReporterInitialized)
        Utils::CrashReporter::DeleteInstance();
}

void    Manager::AddEngine(IEngine *engine, const Permissions &permissions)
{
// verifications, si l'engine existe deja ou est null
    if (engine == NULL)
        throw Utils::Exception("Engine::Manager", "The engine is null");

    std::string name = engine->ResolvedClassName();
    MapEngine::iterator it = _mapEngine.find(name);
    if (it != _mapEngine.end())
        throw Utils::Exception("Engine::Manager", "The engine '" + name + "' already exist");

    _mapEngine[name] = AllowedEngine(engine, permissions);
}

void    Manager::RemoveEngine(const std::string &name, bool del)
{
    MapEngine::iterator it = _mapEngine.find(name);
    if (it != _mapEngine.end())
    {
        it->second.engine->Stop();
        it->second.engine->Wait();
        if (del && it->second.engine->DeletePriority() > 0)
            delete it->second.engine;
        _mapEngine.erase(it);
    }
}

IEngine *Manager::GetEngine(const std::string &name)
{
    MapEngine::iterator it = _mapEngine.find(name);
    if (it == _mapEngine.end())
        throw Utils::Exception("Engine::Manager", "'" + name + "' Don't exist !");
    return it->second.engine;
}

void    Manager::Start()
{
// lancement des threads
    System::Locker l(&_mutexGlobal);
    for (MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); ++itEngine)
        itEngine->second.engine->Start();
    _isLaunched = true;
}

void    Manager::Wait()
{
    CALLSTACK_INFO();

// on attend que les thread se finisse
    for (MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); ++itEngine)
        itEngine->second.engine->Wait();
}

void    Manager::Stop()
{
    CALLSTACK_INFO();

    System::Locker l(&_mutexGlobal);
    // check if the current thread is allowed to exit the thread
    unsigned int currentThreadId = System::ThreadId();
    if (_mainThreadId == currentThreadId)
        _isLaunched = false;
    else
    {
        for (MapEngine::iterator itEngine = _mapEngine.begin(); _isLaunched && itEngine != _mapEngine.end(); ++itEngine)
        {
            if (itEngine->second.engine->GetThreadId() == currentThreadId)
            {
                if (itEngine->second.permissions.Enabled(Exit))
                    _isLaunched = false;
            }
        }
    }
}

void Manager::PushEvent(const std::string &engineName, unsigned int id)
{
    try
    {
        GetEngine(engineName)->PushEvent(id);
    }
    catch(const std::exception &e)
    {
        LOG_ERROR << "Error: " << e.what() << std::endl;
    }
}

void Manager::PushEvent(const std::string &engineName, unsigned int id, IEvent *e)
{
    try
    {
        GetEngine(engineName)->PushEvent(id, e);
    }
    catch(const std::exception &e)
    {
        LOG_ERROR << "Error: " << e.what() << std::endl;
    }
}

void Manager::WaitAllEngineStarted()
{
    bool isLaunched = false;
    while (!isLaunched)
    {
        System::Locker l(&_mutexGlobal);
        isLaunched = _isLaunched;
    }
}

void Manager::WaitLoadingContextPriority(unsigned char priority)
{
    unsigned char topPriority = 0xff;

    while (topPriority > priority)
    {
        topPriority = 0;
        System::Locker l(&_mutexGlobal);
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second.engine->ContextLoaded() && it->second.engine->LoadingContextPriority() > topPriority)
                topPriority = it->second.engine->LoadingContextPriority();
    }
}

void Manager::WaitLoadingPriority(unsigned char priority)
{
    unsigned char topPriority = 0xff;
    while (priority < topPriority)
    {
        topPriority = 0;
        System::Locker l(&_mutexGlobal);
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second.engine->Loaded() && it->second.engine->LoadingPriority() > topPriority)
                topPriority = it->second.engine->LoadingPriority();
    }
}

void Manager::WaitEnginesContextLoading()
{
    bool state = false;
    while (!state && IsLaunched())
    {
        state = true;
        System::Locker l(&_mutexGlobal);
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second.engine->ContextLoaded())
                state = false;
    }
}

void Manager::WaitEnginesLoading()
{
    bool state = false;
    while (!state && IsLaunched())
    {
        state = true;
        System::Locker l(&_mutexGlobal);
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second.engine->Loaded())
                state = false;
    }
}

void Manager::WaitReleasePriority(unsigned int priority)
{
	unsigned char topPriority = 0;
    while (priority > topPriority)
    {
        topPriority = 0xff;
        {
            System::Locker l(&_mutexGlobal);
            for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
                if (!it->second.engine->Released() && it->second.engine->DeletePriority() < topPriority)
                    topPriority = it->second.engine->LoadingPriority();
        }
		if (priority > topPriority)
			System::Sleep(0);
	}
}
