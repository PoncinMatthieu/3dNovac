
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

        /**
            Member function pointer to receive an event
            \param e could be NULL
        */
        typedef void (EventManager::*CmdFunction)(IEvent *e);
        /**
            Member function pointer to receive an event string (from the console)
            \param e could be NULL
        */
        typedef void (EventManager::*CmdFunctionString)(EventString *e);

        /// Base class to define an engine event
        struct LCORE IEvent
        {
            virtual ~IEvent()       {}

            /**
                \return the data of type T of the event.
                Use a dynamic_cast with the template class Event<T> to return the data,
                so the method throw an exception if the event is not of type Event<T>.
            */
            template<typename T>
            T   &GetData();

            /** \return the typeid of the event data, to debug and know what type of event you received. */
            virtual const std::type_info    &GetDataTypeId() = 0;
        };

        /// Define an event, simple container used to stock a data with an event
        template<typename T>
        struct Event : public IEvent
        {
            Event(const T &d)  : Data(d)   {}

            /** \return the typeid of the event data, to debug and know what type of event you received. */
            virtual const std::type_info    &GetDataTypeId();

            T   Data;           ///< data of type T
        };

        /// Define an event string
        /**
            Used to send event witch contain a string. Principaly used to manage the console events
        */
        struct LCORE EventString : public IEvent
        {
            EventString(const std::string &s)    : args(s)     {}

            /** Split the argument of the event with the given delimiter, to get a sub string argument */
            template<typename T>
            void NextArg(T &arg, const std::string delimit = " ");

            /** \return the typeid of the event data, to debug and know what type of event you received. */
            virtual inline const std::type_info    &GetDataTypeId()        {return typeid(std::string);}

            std::string     args;           ///< the string argument of the event
        };

        /// Define a Cmd handler witch have an id and a member function pointer
        struct LCORE Cmd
        {
            Cmd(unsigned int i, CmdFunction f) : id(i), function(f)    {}

            unsigned int        id;             ///< Id of the command
            CmdFunction         function;       ///< The member function pointer to excute the handler event
        };

        /// Define a CmdString handler witch have a name, a comment to describe the cmd and a member function pointer (used for the console cmds)
        struct LCORE CmdString
        {
            CmdString(const std::string &n, const std::string &c, CmdFunctionString f) : name(n), comment(c), function(f)    {}

            std::string         name;           ///< Name of the command
            std::string         comment;        ///< Comment of the command (displayed with the command help)
            CmdFunctionString   function;       ///< The member function pointer to excute the handler event
        };

        /// Base class used to manage events, can receive event and execute it
        class LCORE EventManager : public System::Object
        {
            public:
                NC_SYSTEM_DEFINE_OBJECT(Nc::System::Object, Nc::Engine::EventManager);

            private:
                typedef std::queue<std::pair<unsigned int, IEvent*> >       QueueEvent;
                typedef std::queue<std::pair<std::string, EventString*> >   QueueEventString;
                typedef std::list<Cmd>                                      ListCmd;
                typedef std::list<CmdString>                                ListCmdString;

            public:
                EventManager();
                virtual ~EventManager();

                /** Push an EventString (from console) without arg */
                void    PushEvent(const std::string &cmdName);
                /** Push an EventString (from console) with an argument string */
                void    PushEvent(const std::string &cmdName, const std::string &args);

                /** Push an IEvent* */
                void    PushEvent(unsigned int id, IEvent *e = NULL);
                /** Push an Event<T> with an argument */
                template<typename T>
                void    PushEvent(unsigned int id, const T &arg);

            protected:
                /** Execute all event present in the event queues */
                void    ExecuteEvents();

                /** Create a new command to call for an event */
                void    AddNewCmd(unsigned int id, CmdFunction function);
                void    AddNewCmd(const std::string &name, const std::string &comment, CmdFunctionString function); // for the console

                bool    _execEvents;        ///< to pause the execution of events in the event queue
                bool    _receiveEvents;     ///< to bypass events (stop the reception of events)

            private:
                /** Execute an event */
                void    ExecuteEvent(unsigned int id, IEvent *e);
                /** Excute an event string */
                void    ExecuteEvent(const std::string &name, EventString *e);

                /** display the list of commands string (for the console) */
                void    Help(EventString *);


                QueueEvent              _queueEvent;                ///< queue of IEvent to execute when we call `ExecuteEvents`
                QueueEventString        _queueEventString;          ///< queue of EventString to execute when we call `ExecuteEvents`

                ListCmd                 _listCmd;                   ///< list of Event handler to use for the management of events
                ListCmdString           _listCmdString;             ///< list of EventString handler to use for the management of event strings

                System::Mutex           _mutexQueue;                ///< protect the mutex queue

                friend LCORE std::ostream& operator << (std::ostream& Out, const EventManager& o);
        };


// templates implementation
        template<typename T>
        T   &IEvent::GetData()
        {
            if (this == NULL)
                throw Utils::Exception("IEvent:GetData", "Event if Null, Event<" + std::string(typeid(T).name()) + "> expected");
            Event<T> *e = dynamic_cast<Event<T>*>(this);
            if (e == NULL)
                throw Utils::Exception("IEvent:GetData", "Bad param, Event<" + std::string(typeid(T).name()) + "> expected");
            return e->Data;
        }

        template<typename T>
        const std::type_info    &Event<T>::GetDataTypeId()
        {
            return typeid(T);
        }

        template<typename T>
        void EventString::NextArg(T &arg, const std::string delimit)
        {
            if (!Utils::Convert::SplitStringTo(args, delimit, arg))
                throw Utils::Exception("EventString::NextArg", "Failed to get the next argument.");
        }

        template<typename T>
        void    EventManager::PushEvent(unsigned int id, const T &arg)
        {
            _mutexQueue.Lock();
            if (_receiveEvents)
                _queueEvent.push(std::pair<unsigned int, IEvent*>(id, new Event<T>(arg)));
            _mutexQueue.Unlock();
        }
    }
}

#endif
