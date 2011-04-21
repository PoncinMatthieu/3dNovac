
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

    File Created At:        26/03/2010
    File Author(s):         Poncin Matthieu

-----------------------------------------------------------------------------*/

#ifndef NC_CORE_SYSTEM_OBJECT_H_
#define NC_CORE_SYSTEM_OBJECT_H_

#include <string>
#include "../Define.h"

namespace Nc
{
    namespace System
    {
        /// base class, with an Id and a Name, the Id is unique
        class LCORE  Object
        {
            public:
                Object();
                Object(const std::string &name);
                virtual ~Object();

                /** Set the name of the object */
                inline void Name(const std::string &name)       {_name = name;}

                /** Return the name of the object */
                inline const std::string &Name() const		    {return _name;}

                /** Return the Id of the object */
                inline unsigned int Id() const			        {return _id;}

                friend std::ostream& operator << (std::ostream& Out, const Object &o);

            protected:
                std::string         _name;

            public:
                unsigned int        _id;
                static unsigned int _nbObject;
        };
    }
}

#endif // OBJECT_H_INCLUDED
