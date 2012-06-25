
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

IEngine::IEngine(Manager *manager, const Utils::Mask<Nc::Engine::Pattern> &pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority)
    : EventManager(), _manager(manager), _loaded(false), _released(false), _pattern(pattern),
      _deletePriority(deletePriority), _loadingContextPriority(loadingContextPriority), _loadingPriority(loadingPriority), _context(NULL),
      _elapsedTime(0), _limitFPS(0), _stop(false), _threadId(0), _requestedToDisableContext(false)
{
}

IEngine::~IEngine()
{
}

void IEngine::Run()
{
    try
    {
        _threadId = System::ThreadId();
        Loading();			// loading ressources and contexts by using the loading priorities
        MainLoop();			// main loop of the thread
		Releasing();		// release the ressources by using the deleting priorities
	}
    catch (const std::exception &e)
    {
        LOG_ERROR << "FATAL Error on " << *this << ": " << e.what() << endl;
/// \todo _manager->Stop() might not work on a fatal error so we exit
//        _manager->Stop();
        exit(-1);
    }
}

void IEngine::MainLoop()
{
    while (_manager->IsLaunched() && !_stop)
    {
        Process();
	}
}

void IEngine::Loading()
{
    _manager->WaitAllEngineStarted();

    // Load context
    if (_loadingContextPriority > 0)
    {
        _manager->WaitLoadingContextPriority(_loadingContextPriority); // waiting for our turn to load the context
        _manager->RequestDisableEveryContext(); // ask to disable every context before creating one
        _manager->MutexGlobal().Lock();
        try
        {
            LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-------------------" << endl;
            CreateContext();
            _pattern.Enable(ContextIsLoaded);
            LOG_DEBUG << "---------------"<< *this << "-:-" << "Create-Context-done--------------" << endl;
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR while creating context: " << e.what() << std::endl;
            exit(-1);
        }
        _manager->MutexGlobal().Unlock();
        _manager->RequestActiveEveryContext(); // let the engines to manage their contexts
        _manager->WaitEnginesContextLoading();	            // waiting for others context
    }

    // Load
    if (_loadingPriority > 0)
    {
        _manager->WaitLoadingPriority(_loadingPriority);        // waiting for our turn to load contents
        _manager->MutexGlobal().Lock();
        try
        {
            if (_context != NULL)
                _context->Active();
            LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-----------------------" << endl;
            LoadContent();
            _loaded = true;
            LOG_DEBUG << "------------------"<< *this << "-:-" << "Loading-done------------------" << endl;

            if (_context != NULL)
                _context->Disable();
        }
        catch (const std::exception &e)
        {
            LOG << "FATAL ERROR while Loading contents: " << e.what() << std::endl;
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
    // if the engine has a context, check if the context is enable
    if (_context != NULL)
    {

        // wait until the manager reactivate the context
        bool st = true;
        while (_requestedToDisableContext)
        {
            // disable the context if we asked for it
            if (st && _context->CurrentThreadId() != 0)
            {
                LOG << *this << " disable context" << std::endl;
                _context->Disable();
                st = false;
            }
            System::Sleep(0);
        }

        unsigned int id = _context->CurrentThreadId();
        if (_threadId != id)
        {
            if (id != 0)
            {
                // request to disable the context in his current thread, and wait for it
//                _manager->RequestDisableContext(id);

                LOG << "/!\\ context active into another thread" << std::endl;

            }
            LOG << "active context" << std::endl;
            _context->Active();
        }
    }

    _sleepMutex.Lock();
    if (_pattern.Enabled(Synchronized))
        _manager->MutexGlobal().Lock();


    try
    {
        #ifdef _DEBUG_THREAD_ENGINE
            LOG_DEBUG <<"Execute `" << *this << "` pid = " << System::ThreadId() << "\n";
        #endif
            ExecuteEvents();
            Execute(_elapsedTime);
        #ifdef _DEBUG_THREAD_ENGINE
            LOG_DEBUG <<"Execute END `" << *this << "` pid = " << System::ThreadId() << "\n";
        #endif
    }
    catch (const std::exception &e)
    {
        LOG_ERROR << "Error on " << *this << ": " << e.what() << std::endl;
    }

//_context->Disable();

    if (_pattern.Enabled(Synchronized))
        _manager->MutexGlobal().Unlock();
    _sleepMutex.Unlock();
    LimitFrameRate();
    _elapsedTime = (float)_clock.ElapsedTime();
    _clock.Reset();
}

void IEngine::LimitFrameRate()
{
    if (_limitFPS > 0)
    {
        float n = (1.f / _limitFPS) - (float)_clock.ElapsedTime();
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

void    IEngine::RequestDisableContext()
{
    _requestedToDisableContext = true;
}

void    IEngine::RequestActiveContext()
{
    _requestedToDisableContext = false;
}
