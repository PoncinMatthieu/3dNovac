
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
#include "../Utils/Logger.h"
#include "Manager.h"

using namespace std;
using namespace Nc;
using namespace Nc::Engine;

IEngine::IEngine(const char *className, const std::string &name, Manager* manager, const Utils::Mask<Nc::Engine::Pattern> &pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : EventManager(className, name), _manager(manager), _loaded(false), _released(false), _pattern(pattern),
      _deletePriority(deletePriority), _loadingContextPriority(loadingContextPriority), _loadingPriority(loadingPriority),
      _elapsedTime(0), _limitFPS(0), _stop(false)
{
}

IEngine::~IEngine()
{
}

void IEngine::Run()
{
    try
    {
        Loading();			// loading ressources and contexts by using the loading priorities
        MainLoop();			// main loop of the thread
		Releasing();		// release the ressources by using the deleting priorities
	}
    catch (const std::exception &e)
    {
        LOG_ERROR << "FATAL Error on " << *this << ": " << e.what() << endl;
///\todo _manager->Stop() don't work on a fatal error so we exit
//        _manager->Stop();
        exit(-1);
    }
}

void IEngine::MainLoop()
{
    while (_manager->IsLaunched() && !_stop)
        Process();
}

void IEngine::Loading()
{
    _manager->WaitAllEngineStarted();
    // if we have a context
    if (_pattern.Enabled(HasAContext) && _loadingContextPriority > 0)
    {
        _manager->WaitLoadingContextPriority(_loadingContextPriority); // waiting for our turn to load the context
        _manager->MutexGlobal().Lock();
        try
        {
            LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-------------------" << endl;
            CreateContext();
            DisableContext();
            _pattern.Enable(ContextIsLoaded);
            LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-done--------------" << endl;
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR during creating context : " << e.what() << std::endl;
            exit(-1);
        }
        _manager->MutexGlobal().Unlock();
        if (_pattern.Disabled(DontWaitOthersContext))
            _manager->WaitEnginesContextLoading();	            // waiting for others context
    }

    if (_loadingPriority > 0)
    {
        _manager->WaitLoadingPriority(_loadingPriority);        // waiting for our turn to load contents
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
        if (_pattern.Enabled(WaitingLoadContentsOfOthersEngines))
            _manager->WaitEnginesLoading();                     // waiting for others loading content engines
    }
}

void IEngine::Releasing()
{
	_manager->WaitReleasePriority(_deletePriority);
	ReleaseContent();
	_released = true;
}

void IEngine::Process()
{
    _sleepMutex.Lock();
    if (_pattern.Enabled(Synchronize))
        _manager->MutexGlobal().Lock();
    if (_pattern.Enabled(HasAContext))
        ActiveContext();

#ifdef _DEBUG_THREAD_ENGINE
    LOG <<"Execute `" << *this << "` pid = " << Utils::System::ThreadId() << "\n";
#endif
    ExecuteEvents();
    Execute(_elapsedTime);
#ifdef _DEBUG_THREAD_ENGINE
    LOG <<"Execute END `" << *this << "` pid = " << Utils::System::ThreadId() << "\n";
#endif
    if (_pattern.Enabled(HasAContext))
        DisableContext();
    if (_pattern.Enabled(Synchronize))
        _manager->MutexGlobal().Unlock();
    _sleepMutex.Unlock();
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

void    IEngine::Sleep()
{
    _sleepMutex.Lock();
}

void    IEngine::WakeUp()
{
    _sleepMutex.Unlock();
}

void    IEngine::Stop()
{
    _stop = true;
}
