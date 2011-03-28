
/*-----------------------------------------------------------------------------

	3dNovac Core
	Copyright (C) 2010-2011, The The 3dNovac Team

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

    File Created At:        10/06/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------

                Implementation de la classe abstraite "Handle"

    permet au classes filles de generer des evenements sur les engines voulu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_ENGINE_HANDLER_H_
#define NC_CORE_ENGINE_HANDLER_H_

#include "../Define.h"
#include "../Utils/Utils.h"

namespace Nc
{
    namespace Engine
    {
        class LCORE Handler
        {
            public:
                Handler();
                Handler(const std::string &engineName, unsigned int id);

                inline void HandlerEngineName(const std::string &name)      {_engineName = name;}
                inline void HandlerId(unsigned int id)                      {_idCmd = id;}

                inline const std::string    &HandlerEngineName()            {return _engineName;}
                inline unsigned int         HandlerId()                     {return _idCmd;}

            protected:
                std::string     _engineName;
                unsigned int    _idCmd;
        };
    }
}

#endif //HANDLE_H_INCLUDED
