
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

    File Created At:        07/09/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_ENGINE_H_
#define NC_CORE_ENGINE_ENGINE_H_

#include <iostream>
#include <list>
#include "../Define.h"
#include "../System/API/API.h"
#include "../Utils/Clock.h"
#include "../Utils/Mask.h"
#include "EventManager.h"

namespace Nc
{
    namespace Engine
    {
        class Manager;

        enum Pattern
		{
			Nop = 									0,			///< do nothing.
			Synchronized =                          1 << 0,     ///< define if we need to lock/unlock a mutex in the loop to synchronize the engine with an other.
			HasAContext =                           1 << 1,     ///< define if we need to create and call `Active/DisableContext` at each loop.
			ContextIsLoaded =                       1 << 2,     ///< informe that the context is loaded, /!\ DON'T USE OUTSIDE OF THE CLASS.
			WaitingLoadContentsOfOthersEngines =    1 << 3,     ///< force the waiting of the others engines afers to have loaded the contents.
			DontWaitOthersContext =                 1 << 4      ///< don't wait the others context (used by the graphic engine for displaying while the loading).
        };

        /// Interface to define a Threaded Engine.
        /**
            Inherit of EventManager, so an engine has a set of routine to receive events from another Engine (Thread).

			To create your own engine, you should at least redefine the method Execute.
			And usualy you will have to redefine the following methods:
				- CreateContext: to add a context to your engine.
				- ActiveContext: to active the context.
				- DisableContext: to disable the context.
				- LoadContent: to load your contents after the creation of the context.
				- ReleaseContent: to realease your contents before the thread terminate.

			An engine has it's main loop executing forever until you call the Stop method, or the manager ask to stop the engine.
			You can have a control over the frequency of execution by calling the method 'LimitFrameRate'.

            An engine can contain a "Context" which will be created by calling the method CreateContext before loading contents.
			You should redefine the CreateContext method and ActiveContext/DisableContext methods to put a context on your engine.

            An engine has a set of priority in the loading and deleting process.
            So in the loading process, with highest priority, the engine will be loaded at first,
            and in the deleting process, with highest priority, the engine will be the last one to be deleted.

            \sa
                - Manager, which is used to contain engines, manage them and forward events bettween engines.
                - MainEngine, which can be used to manage an engine receiving inputs, like a game engine.
                - EventManager, base class of IEngine used to allow events between engines.
		*/
        class LIB_NC_CORE IEngine : public EventManager, public System::Thread
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::Engine::EventManager, Nc::Engine::IEngine);

            public:
                // constructeur et destructeur
                IEngine(Manager *manager, const Utils::Mask<Pattern> &pattern, unsigned char deletePriority, unsigned char loadingContextPriority, unsigned int loadingPriority);
                virtual ~IEngine();

                /** Set the manager. */
                inline void             SetManager(Manager *m)          {_manager = m;}

                /** Call the methods `Loading` and `MainLoop` of the engine. */
                virtual void            Run();
                /** Sleep the engine. */
                void                    Sleep();
                /** Wake up the engine. */
                void                    WakeUp();
                /** Stop the engine without stoping the manager. */
                void                    Stop();

                /** Set the limit frame rate of the thread. If the limit is not null, the thread will be sleeped to have good frame rate. */
                inline void             LimitFrameRate(unsigned int limit)     {_limitFPS = limit;}

                /** Load the contents of the thread before entering in the `MainLoop`. */
                virtual inline void     LoadContent()                   {}
				/** Release the contents of the thread just after the `MainLoop`. */
                virtual inline void     ReleaseContent()				{}

                /** Execute the engine, to be redefine. */
                virtual void            Execute(float runningTime) = 0;

                /** \return true if the context is loaded or if there are no context. */
                inline bool             ContextLoaded()                 {return (_pattern.Disabled(HasAContext) || _pattern.Enabled(ContextIsLoaded));}
                /** \return true if the engine is loaded. */
                inline bool             Loaded() const                  {return _loaded;}
				/** \return true if the content of the engine has been released. */
				inline bool				Released() const				{return _released;}
				/** \return the priority delete. */
                unsigned char           DeletePriority()                {return _deletePriority;}
                /** \return the context priority loading. */
                unsigned char           LoadingContextPriority()        {return _loadingContextPriority;}
                /** \return the priority loading. */
                unsigned char           LoadingPriority()               {return _loadingPriority;}

                /** \return the pattern of the engine. */
                const Utils::Mask<Pattern> &GetPattern() const          {return _pattern;}

            protected:
                /** \return the time since the loop iteration started. */
                float                   ElapsedTime() const             {return _clock.ElapsedTime();}

                /** Before to entering in the main loop, load contents and create context by using the engine manager priorities. */
                virtual void            Loading();
                /** Just after the main loop, delete the contents by using the engine manager priorities. */
                virtual void            Releasing();
                /** The main loop of the thread which call the `Process` method until the Manager will stoped. */
                virtual void            MainLoop();
                /**
					Process the engine.
					Procedure :
						- Active/DisableContext
						- ExecuteEvents
						- Execute
						- Thread synchronisation with mutex
				*/
                virtual void            Process();

                /** Create the context, to redefine. */
                virtual inline void     CreateContext()     {}
                /** Active the context, to redefine. */
                virtual inline void     ActiveContext()     {}
                /** Disable the context, to redefine. */
                virtual inline void     DisableContext()    {}

            private:
                /** Pause the thread to avoid taking 100% of the CPU, if the limit is >0 */
                void                    LimitFrameRate();

            protected:
                Manager                 *_manager;                  ///< The instance of the engine Manager.
                bool                    _loaded;                    ///< true if the engine is loaded.
				bool					_released;					///< true if the engine has been released.

                Utils::Mask<Pattern>    _pattern;                   ///< To detemine the pattern (comportement) of the engine.
                unsigned char           _deletePriority;            ///< if the priority is null, the manager will not delete the Engine but the contents will be even deleted at the end of the thread.
                unsigned char           _loadingContextPriority;    ///< if the priority is null, no context loading.
                unsigned char           _loadingPriority;           ///< if the priority is null, no content loading.

                unsigned int            _limitFPS;                  ///< if > 0, used to sleep the thread with the good values to have the good number of seconds.

            private:
                float                   _elapsedTime;               ///< Elapsed time between 2 frame (Execute) in second.
                Utils::Clock            _clock;                     ///< the clock used to compute the elapsed time and the fps sleep if the limit is set.
                System::Mutex           _sleepMutex;                ///< mutex used to sleep the engine.
                System::Locker          *_sleepLocker;              ///< locker used to sleep the engine.
                bool                    _stop;                      ///< a boolean to stop the engine.
        };
    }
}

#endif
