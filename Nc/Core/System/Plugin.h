
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

    File Created At:        27/03/2011
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_PLUGIN_H_
#define NC_CORE_SYSTEM_PLUGIN_H_

#include "../Define.h"
#include "../Utils/FileName.h"

namespace Nc
{
    namespace System
    {
        class LCORE Plugin
        {
            public:
                Plugin();
                virtual ~Plugin();

                virtual void Load() = 0;

            protected:
                void    Open(const Utils::FileName &file);
                void    Close();
                void    *LoadSymbol(const std::string &sym);

            private:
                void    *_handle;
        };
    }
}

#endif // NC_CORE_SYSTEM_PLUGIN_H_
