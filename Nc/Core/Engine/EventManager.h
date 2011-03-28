
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

    File Created At:        10/11/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

    Event Engine, allow to receive event
    to use with heritance

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_EVENT_MANAGER_H_
#define NC_CORE_ENGINE_EVENT_MANAGER_H_

#include <typeinfo>
#include <list>
#include <queue>
#include "../Define.h"
#include "../System/Object.h"
#include "../System/API/API.h"

namespace Nc
{
    namespace Engine
    {
        class   EventManager;
        struct  IEvent;
        struct  EventString;
        typedef void (EventManager::*CmdFunction)(IEvent *e);                //!\ the param e could be NULL
        typedef void (EventManager::*CmdFunctionString)(EventString *e);


        struct LCORE IEvent
        {
            virtual ~IEvent()       {}

            template<typename T>    T   &GetData();
        };

        template<typename T>
        struct LCORE Event : public IEvent
        {
            Event(const T &d)  : Data(d)   {}

            T   Data;
        };

        template<typename T>
        T   &IEvent::GetData()
        {
            Event<T> *e = dynamic_cast<Event<T>*>(this);
            if (e == NULL)
                throw Utils::Exception("GameNetworkEngine", "GetData: bad param, Event<" + std::string(typeid(T).name()) + "> expected");
            return e->Data;
        }

        struct LCORE EventString : public IEvent
        {
            EventString(const std::string &s)    : args(s)     {}

            template<typename T>
            void NextArg(T &arg, const std::string delimit = " ")
            {
                if (!Utils::Convert::SplitStringTo(args, delimit, arg))
                    throw Utils::Exception("RTypeEngine", "can't convert mobId");
            }

            std::string     args;
        };

        struct LCORE Cmd
        {
            Cmd(unsigned int i, CmdFunction f) : id(i), function(f)    {}

            unsigned int        id;
            CmdFunction         function;
        };

        struct LCORE CmdString
        {
            CmdString(const std::string &n, const std::string &c, CmdFunctionString f) : name(n), comment(c), function(f)    {}

            std::string         name;
            std::string         comment;
            CmdFunctionString   function;
        };

        class LCORE EventManager : public System::Object
        {
            private:
                typedef std::queue<std::pair<unsigned int, IEvent*> >       QueueEvent;
                typedef std::queue<std::pair<std::string, EventString*> >   QueueEventString;
                typedef std::list<Cmd>                                      ListCmd;
                typedef std::list<CmdString>                                ListCmdString;

            public:
                EventManager(const std::string &name);
                virtual ~EventManager();

                void    PushEvent(const std::string &cmdName);                              // Push an EventString (from console) without arg
                void    PushEvent(const std::string &cmdName, const std::string &args);     // Push an EventString (from console) with an argument string

                void    PushEvent(unsigned int id, IEvent *e = NULL);                       // Push an IEvent*
                template<typename T>
                void    PushEvent(unsigned int id, const T &arg);                           // Push an Event<T> with an argument

            protected:
                /// execute all the event queue
                void    ExecuteEvents();

                /// create a new command to call for an event
                void    AddNewCmd(unsigned int id, CmdFunction function);
                void    AddNewCmd(const std::string &name, const std::string &comment, CmdFunctionString function); // for the console

                bool    _execEvents;  // allow to pause the execution of events in the event queue

            private:
                void    ExecuteEvent(unsigned int id, IEvent *e); // execute an event
                void    ExecuteEvent(const std::string &name, EventString *e); // execute an event

                /// display the list of commands string (for the console)
                void    Help(EventString *);

                /// queue of events to exec when we call `ExecuteEvents`
                QueueEvent              _queueEvent;
                QueueEventString        _queueEventString;

                /// list of commands to use for manage events with a method pointeur
                ListCmd                 _listCmd;
                ListCmdString           _listCmdString;

                /// protect the mutex queue
                System::Mutex           _mutexQueue;

                friend LCORE std::ostream& operator << (std::ostream& Out, const EventManager& o);
        };

        template<typename T>
        void    EventManager::PushEvent(unsigned int id, const T &arg)
        {
            _mutexQueue.Lock();
            _queueEvent.push(std::pair<unsigned int, IEvent*>(id, new Event<T>(arg)));
            _mutexQueue.Unlock();
        }
    }
}

#endif
