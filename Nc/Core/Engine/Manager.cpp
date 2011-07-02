
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

#include <signal.h>
#include "Manager.h"
#include "../System/Config.h"
#ifdef SYSTEM_LINUX
	#include <execinfo.h>
#endif

using namespace std;
using namespace Nc;
using namespace Nc::Engine;

MapEngine      Manager::_mapEngine;

Manager::Manager(const Utils::FileName &confFile)
{
    _isLaunched = false;

    // Open the Config file of 3dNovac
    if (!confFile.empty())
    {
        CONFIG.Load(confFile);
        _confFileOpened = true;
    }

    // redirect signal SIGSEGV functions
    set_terminate(Terminate);
    #ifdef SYSTEM_LINUX
    signal(SIGSEGV, &Manager::RecieveSegv);
    #endif
}

Manager::~Manager()
{
// priority destruction ! (if 0, no delete)
    unsigned char lowestPriority = 0xff;
    unsigned char nextPriority;

    // first find the lowest priority
    for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
        if (it->second->DeletePriority() < lowestPriority && it->second->DeletePriority() > 0)
            lowestPriority = it->second->DeletePriority();

    // and next delete all engines with the priority order
    while (lowestPriority != 0 && _mapEngine.size() > 0)
    {
        nextPriority = 0xff;
        MapEngine::iterator it2;
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); it = it2)
        {
            it2 = it;
            ++it2;
            if (it->second->DeletePriority() == lowestPriority)
            {
                delete it->second;
                _mapEngine.erase(it);
            }
            else if (it->second->DeletePriority() < nextPriority)
                nextPriority = it->second->DeletePriority();
        }
        lowestPriority = nextPriority;
    }

// close the Config file of 3dNovac
    if (_confFileOpened)
        CONFIG.DeleteInstance();
}

#ifdef SYSTEM_LINUX
void Manager::RecieveSegv(int)
{
#ifdef _DEBUG
// print the backtrace
    LOG << "Backtrace: " << std::endl;
    void    *array[25];
    int     nSize = backtrace(array, 25);
    char    **symbols = backtrace_symbols(array, nSize);
    for (int i = 0; i < nSize; i++)
        LOG << symbols[i] << std::endl;
    LOG << std::endl;
    free(symbols);
#endif
    throw Utils::Exception("System::Manager", "Receive Segv");
}
#endif

void Manager::Terminate()
{
//reference: http://www.ibm.com/developerworks/linux/library/l-cppexcep.html
     try
     {
         throw; // re-throw
     }
     catch (const Utils::Exception &e)
     {
         LOG << "Fatal error: " << e.what() << std::endl;
     }

     //if this is a thread performing some core activity
     abort();
     // else if this is a thread used to service requests
     // pthread_exit();
}

void Manager::AddEngine(const std::string &name, IEngine *engine)
{
// verifications, si le gameManager n'a pas encore ete lance ou si l'engine existe deja
    if (_isLaunched)
        throw Utils::Exception("Engine::Manager", "Can't add new engine, the game manager is already launch");
    if (engine == NULL)
        throw Utils::Exception("Engine::Manager", "The engine is null");
    MapEngine::iterator it = _mapEngine.find(name);
    if (it != _mapEngine.end())
        throw Utils::Exception("Engine::Manager", "The engine " + name + "already exist");

    _mapEngine[name] = engine;
}

IEngine *Manager::GetEngine(const std::string &name)
{
    MapEngine::iterator it = _mapEngine.find(name);
    if (it == _mapEngine.end())
        throw Utils::Exception("Engine::Manager", "'" + name + "' Don't exist !");
    return it->second;
}

void Manager::Start()
{
// lancement des threads
    _isLaunched = true;
    for (MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); itEngine++)
        itEngine->second->Start();
}

void Manager::Wait()
{
// on attend que les thread se finisse
    for (MapEngine::iterator itEngine = _mapEngine.begin(); itEngine != _mapEngine.end(); itEngine++)
        itEngine->second->Wait();
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


void Manager::PushEvent(const std::string &engineName, const std::string &cmdName)
{
    try
    {
        GetEngine(engineName)->PushEvent(cmdName);
    }
    catch(const std::exception &e)
    {
        LOG_ERROR << "Error: " << e.what() << std::endl;
    }
}

void Manager::PushEvent(const std::string &engineName, const std::string &cmdName, const std::string &args)
{
    try
    {
        GetEngine(engineName)->PushEvent(cmdName, args);
    }
    catch(const std::exception &e)
    {
        LOG_ERROR << "Error: " << e.what() << std::endl;
    }
}

void Manager::WaitLoadingContextPriority(unsigned char priority)
{
    unsigned char topPriority = 0xff;

    while (topPriority > priority)
    {
        topPriority = 0;
        _mutexGlobal.Lock();
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second->ContextLoaded() && it->second->LoadingContextPriority() > topPriority)
                topPriority = it->second->LoadingContextPriority();
        _mutexGlobal.Unlock();
    }
}

void Manager::WaitLoadingPriority(unsigned char priority)
{
    unsigned char topPriority = 0xff;
    while (priority < topPriority)
    {
        topPriority = 0;
        _mutexGlobal.Lock();
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second->Loaded() && it->second->LoadingPriority() > topPriority)
                topPriority = it->second->LoadingPriority();
        _mutexGlobal.Unlock();
    }
}

void Manager::WaitEnginesContextLoading()
{
    bool state = false;
    while (!state && IsLaunched())
    {
        state = true;
        _mutexGlobal.Lock();
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second->ContextLoaded())
                state = false;
        _mutexGlobal.Unlock();
    }
}

void Manager::WaitEnginesLoading()
{
    bool state = false;
    while (!state && IsLaunched())
    {
        state = true;
        _mutexGlobal.Lock();
        for (MapEngine::iterator it = _mapEngine.begin(); it != _mapEngine.end(); ++it)
            if (!it->second->Loaded())
                state = false;
        _mutexGlobal.Unlock();
    }
}
