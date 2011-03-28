
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

#include "IEngine.h"
#include "Manager.h"

using namespace std;
using namespace Nc::Engine;

IEngine::IEngine(const std::string &name, Manager* manager, unsigned int pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : EventManager(name), _manager(manager), _loaded(false), _elapsedTime(0), _limitFPS(0), _pattern(pattern),
      _deletePriority(deletePriority), _loadingContextPriority(loadingContextPriority), _loadingPriority(loadingPriority)
{
}

IEngine::~IEngine()
{
}

void IEngine::Run()
{
    try
    {
        Loading(); // loading ressources and contexts
        MainLoop(); // main loop of the thread
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "FATAL Error on " << *this << ": " << e.what() << endl;
//        _manager->Stop(-1); //TODO: visiblement le stop ne fonctionne pas lorsqu'on catch une exception
        exit(-1);
    }
}

void IEngine::MainLoop()
{
    while (_manager->IsLaunched())
        Process();
}

void IEngine::Loading()
{
    // if we have a context
    if (_pattern & HasAContext && _loadingContextPriority > 0)
    {
        _manager->WaitLoadingContextPriority(_loadingContextPriority); /// waiting for our turn to load the context
        _manager->MutexGlobal().Lock();
        try
        {
                LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-------------------" << endl;
                CreateContext();
                DisableContext();
                _pattern |= ContextIsLoaded;
                LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-done--------------" << endl;
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR during creating context : " << e.what() << std::endl;
            exit(-1);
        }
        _manager->MutexGlobal().Unlock();
        if (!(_pattern & DontWaitOthersContext))
            _manager->WaitEnginesContextLoading();	            /// waiting for others context
    }

    if (_loadingPriority > 0)
    {
        _manager->WaitLoadingPriority(_loadingPriority);        /// waiting for our turn to load contents
        _manager->MutexGlobal().Lock();
        try
        {
            ActiveContext();
            LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-----------------------" << endl;
            LoadContent();
            _loaded = true;
            LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-done------------------" << endl;
            DisableContext();
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR during Loading contents : " << e.what() << std::endl;
            exit(-1);
        }
       _manager->MutexGlobal().Unlock();
        if (_pattern & WaitingLoadContentsOfOthersEngines)
            _manager->WaitEnginesLoading();                     /// waiting for others loading content engines
    }
}

void IEngine::Process()
{
    if (_pattern & Synchronize)
        _manager->MutexGlobal().Lock();
    if (_pattern & HasAContext)
        ActiveContext();

#ifdef _DEBUG_THREAD_ENGINE
    LOG <<"Execute `" << *this << "` pid = " << Utils::System::ThreadId() << "\n";
#endif
    ExecuteEvents();
    Execute(_elapsedTime);
#ifdef _DEBUG_THREAD_ENGINE
    LOG <<"Execute END `" << *this << "` pid = " << Utils::System::ThreadId() << "\n";
#endif
    if (_pattern & HasAContext)
        DisableContext();
    if (_pattern & Synchronize)
        _manager->MutexGlobal().Unlock();
    LimitFrameRate();
    _elapsedTime = _clock.ElapsedTime();
    _clock.Reset();
}

void IEngine::LimitFrameRate()
{
    if (_limitFPS > 0)
    {
        float n = (1.f / _limitFPS) - _clock.ElapsedTime();
        if (n > 0)
            System::Sleep(n);
    }
}
