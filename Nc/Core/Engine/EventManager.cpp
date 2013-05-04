
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
#include "../Utils/Logger.h"
#include "../Utils/FileName.h"

using namespace std;
using namespace Nc::Engine;

EventManager::EventManager()
	: System::Object()
{
    _execEvents = true;
    _receiveEvents = true;
    _throwErrorAtUnknownCommand = true;
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
            Out << o.ResolvedClassName();
            if (!o._name.empty())
                Out << ":" << o._name;
            return Out;
        }
    }
}

void EventManager::AddNewCmd(unsigned int id, CmdFunction function)
{
    _listCmd.push_back(Cmd(id, function));
}

void EventManager::ExecuteEvent(unsigned int id, IEvent *e)
{
    try
    {
        for (ListCmd::iterator it = _listCmd.begin(); it != _listCmd.end(); it++)
        {
            if (it->id == id)
            {
                CALLSTACK_INFO_ARG("Id: " + Utils::Convert::ToString(id) + ((e != NULL) ? std::string(" e: ") + e->GetDataTypeId().name() : ""));
                (this->*(it->function))(e);
                return;
            }
        }
        if (_throwErrorAtUnknownCommand)
            throw Utils::Exception("Unknown command id:" + Utils::Convert::ToString(id));
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << "Error: " << *this << ": " << ex.what() << std::endl;
    }
    catch (...)
    {
        LOG_ERROR << *this << " Unknown Error, throw catched !" << std::endl;
    }
}

void  EventManager::ExecuteEvents()
{
    CALLSTACK_INFO();
    if (_execEvents)
    {
        // exec queue event
        for (; !_queueEvent.empty(); _queueEvent.pop())
        {
            std::pair<unsigned int, IEvent*> p;
            {
                System::Locker l(&_mutexQueue);
                p = _queueEvent.front();
            }
            ExecuteEvent(p.first, p.second);
            if (p.second != NULL)
                delete p.second;
        }
    }
}

void    EventManager::PushEvent(unsigned int id, IEvent *e)
{
    System::Locker l(&_mutexQueue);
    if (_receiveEvents)
        _queueEvent.push(std::pair<unsigned int, IEvent*>(id, e));
}
