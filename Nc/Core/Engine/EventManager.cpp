
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

#include "EventManager.h"

using namespace std;
using namespace Nc::Engine;

EventManager::EventManager(const std::string &name) : Object(name)
{
    _execEvents = true;
    AddNewCmd("help", "display help", (CmdFunctionString)&EventManager::Help);
}

EventManager::~EventManager()
{
}

namespace Nc
{
    namespace Engine
    {
        std::ostream& operator << (std::ostream& Out, const EventManager& o)
        {
            Out << o._name;
            return Out;
        }
    }
}

void EventManager::Help(EventString *)
{
    LOG << "List all function of " << *this << endl;
    for (ListCmdString::iterator it = _listCmdString.begin(); it != _listCmdString.end(); it++)
        LOG << it->name << "--> " << it->comment << endl;
}

void EventManager::AddNewCmd(unsigned int id, CmdFunction function)
{
    _listCmd.push_back(Cmd(id, function));
}

void EventManager::AddNewCmd(const std::string &name, const std::string &comment, CmdFunctionString function)
{
    _listCmdString.push_back(CmdString(name, comment, function));
}

void EventManager::ExecuteEvent(unsigned int id, IEvent *e)
{
    try
    {
        for (ListCmd::iterator it = _listCmd.begin(); it != _listCmd.end(); it++)
        {
            if (it->id == id)
            {
                (this->*(it->function))(e);
                return;
            }
        }
        throw Utils::Exception(_name, "Unknown command id:" + id);
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << "Error : "<< ex.what() << std::endl;
    }
    catch (...)
    {
        LOG_ERROR << "Unknown Error, throw catched !" << std::endl;
    }
}

void EventManager::ExecuteEvent(const std::string &name, EventString *e)
{
    try
    {
        for (ListCmdString::iterator it = _listCmdString.begin(); it != _listCmdString.end(); it++)
        {
            if (it->name == name)
            {
                (this->*(it->function))(e);
                return;
            }
        }
        throw Utils::Exception(_name, "Unknown command `" + name + "`");
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << "Error : "<< ex.what() << std::endl;
    }
    catch (...)
    {
        LOG_ERROR << "Unknown Error, throw catched !" << std::endl;
    }
}

void  EventManager::ExecuteEvents()
{
    if (_execEvents)
    {
        // exec queue event
        for (; !_queueEvent.empty(); _queueEvent.pop())
        {
            _mutexQueue.Lock();
            std::pair<unsigned int, IEvent*> p = _queueEvent.front();
            _mutexQueue.Unlock();
            ExecuteEvent(p.first, p.second);
            if (p.second != NULL)
                delete p.second;
        }

        // exec queue event string
        for (; !_queueEventString.empty(); _queueEventString.pop())
        {
            _mutexQueue.Lock();
            std::pair<std::string, EventString*> p = _queueEventString.front();
            _mutexQueue.Unlock();
            ExecuteEvent(p.first, p.second);
            if (p.second != NULL)
                delete p.second;
        }
    }
}

void    EventManager::PushEvent(unsigned int id, IEvent *e)
{
    _mutexQueue.Lock();
    _queueEvent.push(std::pair<unsigned int, IEvent*>(id, e));
    _mutexQueue.Unlock();
}

void EventManager::PushEvent(const std::string &cmdName)
{
    _mutexQueue.Lock();
    _queueEventString.push(std::pair<std::string, EventString*>(cmdName, (EventString*)NULL));
    _mutexQueue.Unlock();
}

void EventManager::PushEvent(const std::string &cmdName, const std::string &args)
{
    _mutexQueue.Lock();
    _queueEventString.push(std::pair<std::string, EventString*>(cmdName, new EventString(args)));
    _mutexQueue.Unlock();
}
